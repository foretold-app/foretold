const { Model } = require('./model');
const models = require('../models');

const { Op } = require('sequelize');

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

    this.Op = Op;
    this.in = Op.in;
    this.gt = Op.gt;
    this.gte = Op.gte;
    this.lt = Op.lt;
    this.lte = Op.lte;
    this.and = Op.and;
    this.not = Op.not;
    this.notIn = Op.notIn;

    this.fn = this.sequelize.fn;
    this.col = this.sequelize.col;
    this.literal = this.sequelize.literal;
  }

  /**
   * @todo: Use ORM opportunities to join tables.
   * @param {Models.ObjectID} [agentId]
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
   * @param {Models.ObjectID} [agentId]
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
   * @param {Models.ObjectID} [agentId]
   * @return {string}
   */
  taggedMeasurements(agentId) {
    return `(
      SELECT "taggedMeasurementId"
      FROM "Measurements"
      WHERE "agentId" = '${agentId}'
      AND "taggedMeasurementId" IS NOT NULL
    )`;
  }

  /**
   * @todo: see this.channelIds()
   * @param {Models.ObjectID} [agentId]
   * @return {string}
   */
  taggedMeasurementsLiteral(agentId) {
    return this.literal(this.taggedMeasurements(agentId));
  }

  /**
   * @todo: see this.channelIds()
   * @param {Models.ObjectID} [agentId]
   * @return {Sequelize.literal}
   */
  measurableIdsLiteral(agentId) {
    return this.literal(this.measurableIds(agentId));
  }

  /**
   * @param {object} [where]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @return {object}
   */
  applyRestrictions(where = {}, restrictions = {}) {
    if (restrictions.isAdmin) return where;
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

    return where;
  }

  /**
   * @param {object} [include]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   */
  applyRestrictionsIncluding(include = [], restrictions = {}) {
    if (!include) include = [];

    // @todo: It is a filter, but not restriction
    if (restrictions.measuredByAgentId) {
      include.push({
        model: this.models.Measurement,
        as: 'Measurements',
        where: { agentId: restrictions.measuredByAgentId },
      });
    }

    return include;
  }

  /**
   * @param {object} [pagination]
   * @param {number} pagination.first
   * @param {string} pagination.after
   * @param {number} pagination.last
   * @param {string} pagination.before
   * @param {number} pagination.limit
   * @param {number} pagination.offset
   * @param {number} total
   * @return {{offset: number, limit: number }}
   */
  getPagination(pagination = {}, total = 0) {
    pagination.before = Math.abs(pagination.before) || total;
    pagination.after = Math.abs(pagination.after) || 0;
    pagination.last = Math.abs(pagination.last) || 0;
    pagination.first = Math.abs(pagination.first) || 0;

    let offset, limit;
    if (pagination.first) limit = pagination.first;
    if (pagination.after) offset = pagination.after + 1;

    if (!offset && !limit) {
      if (pagination.last) {
        limit = pagination.last;
        offset = pagination.before - pagination.last;
      } else if (pagination.before !== total) {
        limit = pagination.before;
      }
    }

    offset = offset || 0;
    if (limit > total) limit = total;
    if (offset < 0) {
      limit += offset;
      offset = 0;
    }
    if (limit < 0) limit = 0;

    return { limit, offset };
  }

  /**
   * @param {*[]} [data]
   * @param {object} [edgePagination]
   * @return {*[]}
   */
  setIndexes(data = [], edgePagination = {}) {
    return data.map((item, index) => {
      item.index = edgePagination.offset + index;
      return item;
    });
  }

  /**
   * @protected
   * @param {object} [where]
   * @param {object} [filter]
   * @param {string[]} [filter.isArchived]
   * @param {Models.ObjectID} [filter.userId]
   */
  applyFilter(where = {}, filter = {}) {
    if (!where) where = {};

    if (filter.isArchived) {
      where.isArchived = {
        [this.in]: this.getBooleansOfList(filter.isArchived),
      };
    }

    if (filter.userId) {
      where.userId = filter.userId;
    }

    return where;
  }

  /**
   * @protected
   * @param {*[]} list
   * @return {*[]}
   */
  getBooleansOfList(list = []) {
    return list.map(item => {
      if (item === 'TRUE') {
        return true;
      } else if (item === 'FALSE') {
        return false;
      }
      return item;
    });
  }

  /**
   * @param {object} [data]
   * @return {Promise.<object>}
   */
  async createOne(data = {}) {
    return await this.model.create(data);
  }

  /**
   * @param {object} [params]
   * @param {object} [data]
   * @return {Promise.<object>}
   */
  async updateOne(params = {}, data = {}) {
    const entity = await this.model.findOne({
      where: params,
    });
    if (entity) {
      await entity.update(data);
    }
    return entity;
  }

  /**
   * @param {object} params
   * @param {object} data
   * @return {boolean}
   */
  async updateAll(params = {}, data = {}) {
    return !!(await this.model.update(
      data,
      { where: params },
    ));
  }

  /**
   * @param {object} filter
   * @param {object} [pagination]
   * @param {object} [restrictions]
   * @return {Promise<void>}
   */
  async getAll(filter = {}, pagination = {}, restrictions = {}) {
    const where = {};
    this.applyRestrictions(where, restrictions);
    return await this.model.findAll({
      limit: pagination.limit,
      offset: pagination.offset,
      where,
    });
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   * @param {Layers.AbstractModelsLayer.pagination} [pagination]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @return {Promise<{data: Models.Model[], total: number}>}
   */
  async getAllWithConnections(filter = {}, pagination = {}, restrictions = {}) {
    const where = {};
    const include = [];

    this.applyRestrictions(where, restrictions);
    this.applyRestrictionsIncluding(include, restrictions);
    this.applyFilter(where, filter);

    const cond = { where, include };

    /** @type {number} */
    const total = await this.model.count(cond);
    const edgePagination = this.getPagination(pagination, total);

    const options = {
      ...cond,
      limit: edgePagination.limit,
      offset: edgePagination.offset,
      order: [['createdAt', 'DESC']],
    };

    /** @type {Models.Model[]} */
    let data = await this.model.findAll(options);
    data = this.setIndexes(data, edgePagination);
    data.total = total;

    return { data, total };
  }

  /**
   * @param {object} params
   * @param {object} [query]
   * @param {object} [restrictions]
   * @return {Promise<Models.Model>}
   */
  async getOne(params = {}, query = {}, restrictions = {}) {
    const where = { ...params };
    const sort = query.sort === 1 ? 'ASC' : 'DESC';
    const order = [['createdAt', sort]];
    this.applyRestrictions(where, restrictions);
    return await this.model.findOne({
      where,
      order,
    });
  }
}

module.exports = {
  ModelPostgres,
};
