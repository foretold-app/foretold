const { Model } = require('./model');
const models = require('../models');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class ModelPostgres extends Model {

  constructor(
    {
      model,
      sequelize,
    }
  ) {
    super();
    this.model = model;
    this.models = models;
    this.sequelize = sequelize;
    this.Op = this.sequelize.Op;
    this.in = this.sequelize.Op.in;
    this.gt = this.sequelize.Op.gt;
    this.lt = this.sequelize.Op.lt;
    this.and = this.sequelize.Op.and;
    this.fn = this.sequelize.fn;
    this.col = this.sequelize.col;
    this.literal = this.sequelize.literal;
  }

  /**
   *
   * @todo: Use ORM opportunities to join tables.
   *
   * @param {string} [agentId]
   * @return {string}
   */
  channelIds(agentId) {
    return agentId ? `(
      SELECT "Channels"."id" FROM "Channels"
      LEFT OUTER JOIN "ChannelMemberships" ON "Channels".id = "ChannelMemberships"."channelId"
        AND "ChannelMemberships"."agentId" = '${agentId}'
      WHERE "Channels"."isPublic" = TRUE OR "ChannelMemberships"."agentId" IS NOT NULL
    )` : `(
      SELECT "Channels"."id" FROM "Channels"
      WHERE "Channels"."isPublic" = TRUE
    )`;
  }

  /**
   * @todo: see this.channelIds()
   * @param {string} [agentId]
   * @return {Sequelize.literal}
   */
  channelIdsLiteral(agentId) {
    return this.literal(this.channelIds(agentId));
  }

  /**
   * @todo: see this.channelIds()
   * @param {string} [agentId]
   * @return {string}
   */
  measurableIds(agentId) {
    return `(
      WITH channelIds AS (${this.channelIds(agentId)})
      SELECT "Measurables"."id" FROM "Measurables"
      WHERE "Measurables"."channelId" IN (SELECT id FROM channelIds)
    )`;
  }

  /**
   * @todo: see this.channelIds()
   * @param {string} [agentId]
   * @return {Sequelize.literal}
   */
  measurableIdsLiteral(agentId) {
    return this.literal(this.measurableIds(agentId));
  }

  /**
   * @param {object} [where]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   */
  applyRestrictions(where = {}, restrictions = {}) {
    if (!where[this.and]) where[this.and] = [];

    if (restrictions.channelId) {
      where[this.and].push({
        channelId: {
          [this.in]: this.channelIdsLiteral(restrictions.agentId),
        },
      });
    }

    if (restrictions.userId) {
      where[this.and].push({
        userId: restrictions.userId,
      });
    }

    if (restrictions.measurableId) {
      where[this.and].push({
        measurableId: {
          [this.in]: this.measurableIdsLiteral(restrictions.agentId),
        },
      });
    }
  }

  /**
   * @param {object} [include]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   */
  applyRestrictionsIncluding(include = [], restrictions = {}) {
    if (!include) include = [];

    if (restrictions.measuredByAgentId) {
      include.push({
        model: this.models.Measurement,
        as: 'Measurements',
        where: { agentId: restrictions.measuredByAgentId },
      });
    }
  }

  /**
   *
   * @param {object} pagination
   * @param {number} pagination.first
   * @param {string} pagination.after
   * @param {number} pagination.last
   * @param {string} pagination.before
   * @param {number} pagination.limit
   * @param {number} pagination.offset
   * @param {object} orderIn
   * @param {*} orderIn.asc
   * @param {*} orderIn.desc
   * @return {{offset: *, limit: *, order: *}}
   */
  getEdgePagination(pagination, orderIn) {
    let limit, offset, order;

    if (pagination.first) limit = pagination.first;
    if (pagination.first) order = orderIn.asc;
    if (pagination.after) offset = pagination.after;

    if (pagination.last) limit = pagination.last;
    if (pagination.last) order = orderIn.desc;
    if (pagination.before) offset = pagination.before;

    if (!limit) limit = pagination.limit;
    if (!offset) offset = pagination.offset;

    return { limit, offset, order };
  }

  /**
   * @protectedo
   * @param {object} [where]
   * @param {object} [filter]
   * @param {string} [filter.isArchived]
   */
  applyFilter(where = {}, filter = {}) {
    if (!where) where = {};

    if (filter.isArchived) {
      where.isArchived = {
        [this.in]: this.getBooleansOfList(filter.isArchived),
      };
    }
  }

  /**
   * @protected
   * @param list
   * @return {*}
   */
  getBooleansOfList(list) {
    return list.map(item => {
      if (item === 'TRUE') {
        return true;
      } else if (item === 'FALSE') {
        return false;
      }
      return item;
    });
  }
}

module.exports = {
  ModelPostgres,
};
