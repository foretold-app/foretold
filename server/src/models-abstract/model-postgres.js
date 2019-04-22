const { Model } = require('./model');

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
   * @param {string} measuredByAgentId
   * @return {string}
   */
  measurableIdsByMeasurements(measuredByAgentId) {
    return `(
      SELECT DISTINCT "Measurements"."measurableId" FROM "Measurements"
      WHERE "Measurements"."agentId" = '${measuredByAgentId}'
    )`;
  }

  /**
   * @todo: see this.channelIds()
   * @param {string} measuredByAgentId
   * @return {string}
   */
  measurableIdsByMeasurementsLiteral(measuredByAgentId) {
    return this.literal(this.measurableIdsByMeasurements(measuredByAgentId));
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

    if (restrictions.measuredByAgentId) {
      where[this.and].push({
        id: {
          [this.in]: this.measurableIdsByMeasurementsLiteral(restrictions.measuredByAgentId),
        },
      });
    }
  }

  /**
   * @param {object} [where]
   * @param {object} [filter]
   * @param {string} [filter.after]
   * @param {string} [filter.before]
   */
  applyCursors(where = {}, filter = {}) {
    if (!where[this.and]) where[this.and] = [];

    if (filter.after) {
      where[this.and].push({
        createdAt: {
          [this.gt]: new Date(filter.after * 1),
        },
      });
    }

    if (filter.before) {
      where[this.and].push({
        createdAt: {
          [this.lt]: new Date(filter.before * 1),
        },
      });
    }
  }
}

module.exports = {
  ModelPostgres,
};
