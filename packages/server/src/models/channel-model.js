const _ = require('lodash');

const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.ModelGeneric}
 */
class ChannelModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Channel,
      sequelize: models.sequelize,
    });
  }

  /**
   * @param {Layers.Models.ModelOptions} options
   * @returns {*}
   * @protected
   */
  _getAttributes(options = {}) {
    const include = [[this._membersCountLiteral(), 'membersCount']];

    const isBookmarked = _.get(options, 'attributes.isBookmarked', null);
    if (!!isBookmarked) {
      const agentId = _.get(isBookmarked, 'agentId', null);
      include.push([this._isBookmarkedLiteral(agentId), 'isBookmarked']);
    }

    const bookmarksCount = _.get(options, 'attributes.bookmarksCount', null);
    if (!!bookmarksCount) {
      include.push([this._bookmarkCountLiteral(), 'bookmarksCount']);
    }

    return { include };
  }

  /**
   * @param {Layers.Pagination} pagination
   * @returns {[*, string][]}
   * @protected
   */
  _getOrderFromPagination(pagination) {
    const order = pagination.getOrder();
    const agentId = pagination.getContext('agentId');
    return order.map((item) => ([
      this._switchField(item.field, agentId),
      item.direction,
    ]));
  }

  /**
   * @param {string} name
   * @param {Defs.AgentID} agentId
   * @returns {object | string}
   * @protected
   */
  _switchField(name = '', agentId) {
    if (name === 'membersCount') {
      return this._membersCountLiteral();
    }
    if (name === 'isBookmarked') {
      return this._isBookmarkedLiteral(agentId);
    }
    return name;
  }

  /**
   * @protected
   * @return {Sequelize.literal}
   */
  _membersCountLiteral() {
    return this.literal(this._membersCount());
  }

  /**
   * @protected
   * @param {Defs.AgentID} agentId
   * @return {Sequelize.literal}
   */
  _isBookmarkedLiteral(agentId) {
    return this.literal(this._isBookmarked(agentId));
  }

  /**
   * @protected
   * @return {Sequelize.literal}
   */
  _bookmarkCountLiteral() {
    return this.literal(this._bookmarkCount());
  }

  /**
   * @protected
   * @return {string}
   */
  _membersCount() {
    return `(
        SELECT count(*) as "membersCount"
        FROM "ChannelMemberships"
        WHERE "ChannelMemberships"."channelId" = "Channel"."id"
    )`;
  }

  /**
   * @protected
   * @param {Defs.AgentID} agentId
   * @return {string}
   */
  _isBookmarked(agentId) {
    if (!agentId) {
      return '( SELECT null )';
    }
    return `(
        SELECT CASE WHEN count(*) >= 1 THEN true ELSE false END
        FROM "ChannelBookmarks"
        WHERE "ChannelBookmarks"."channelId" = "Channel"."id"
        AND "ChannelBookmarks"."agentId" = '${agentId}'
        LIMIT 1
    )`;
  }

  /**
   * @protected
   * @return {string}
   */
  _bookmarkCount() {
    return `(
        SELECT count(*) as "bookmarksCount"
        FROM "ChannelBookmarks"
        WHERE "ChannelBookmarks"."channelId" = "Channel"."id"
    )`;
  }
}

module.exports = {
  ChannelModel,
};
