const _ = require('lodash');

const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class ChannelModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Channel,
      sequelize: models.sequelize,
    });
  }

  /**
   * @public
   * @todo: To decline in use of "this.models".
   * @todo: To use "applyFilter" to use "transactions" later.
   * @param {Models.ChannelID} channelId
   * @return {Promise<Model[]>}
   */
  async getAgentsByChannelId(channelId) {
    const channel = await this.model.findOne({
      where: { id: channelId },
      include: [{
        model: this.models.Agent,
        as: 'agents',
      }],
    });
    return _.get(channel, 'agents', []);
  }

  /**
   * @param {Layers.AbstractModelsLayer.options} options
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

    return { include };
  }

  /**
   * @param pagination
   * @returns {[any, any][]}
   * @protected
   */
  _getDefaultOrder(pagination) {
    return pagination.getOrder()
      .map((item) => ([this._switchField(item.field), item.direction]));
  }

  /**
   * @param name
   * @returns {*}
   * @private
   */
  _switchField(name = '') {
    if (name === 'membersCount') {
      return this._membersCountLiteral();
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
   * @return {Sequelize.literal}
   */
  _isBookmarkedLiteral(agentId) {
    return this.literal(this._isBookmarked(agentId));
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
}

module.exports = {
  ChannelModel,
};
