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
   * @todo: see this.channelIds()
   * @protected
   * @param {Models.ObjectID} [agentId]
   * @return {Sequelize.literal}
   */
  channelIdsLiteral(agentId) {
    return this.literal(this.channelIds(agentId));
  }

  /**
   * @todo: Use ORM opportunities to join tables.
   * @protected
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
   * @protected
   * @param {Models.ObjectID} channelId
   * @return {Sequelize.literal}
   */
  agentsIdsLiteral(channelId) {
    return this.literal(this.agentsIds(channelId));
  }

  /**
   * @todo: Use ORM opportunities to join tables.
   * @protected
   * @param {Models.ObjectID} channelId
   * @return {string}
   */
  agentsIds(channelId) {
    return `(
      SELECT "ChannelMemberships"."agentId" FROM "ChannelMemberships"
      WHERE "ChannelMemberships"."channelId" = '${channelId}'
    )`;
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
  taggedMeasurementsLiteral(agentId) {
    return this.literal(this.taggedMeasurements(agentId));
  }

  /**
   * @todo: see this.channelIds()
   * @protected
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
   * @param {object} [where]
   * @protected
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
   * @protected
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
   * @protected
   * @param {object} [where]
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   * @param {Models.ObjectID} [filter.userId]
   */
  applyFilter(where = {}, filter = {}) {
    if (!where) where = {};
    if (!where[this.and]) where[this.and] = [];

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
   * @param {Layers.AbstractModelsLayer.pagination} [pagination]
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
   * @protected
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
   * @param {*[]} [list]
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
   * @public
   * @param {object} [data]
   * @param {Layers.AbstractModelsLayer.restrictions} [_restrictions]
   * @param {Layers.AbstractModelsLayer.options} [_options]
   * @return {Promise.<object>}
   */
  async createOne(data = {}, _restrictions = {}, _options = {}) {
    return this.model.create(data);
  }

  /**
   * @public
   * @param {object} [params]
   * @param {object} [data]
   * @param {Layers.AbstractModelsLayer.restrictions} [_restrictions]
   * @param {Layers.AbstractModelsLayer.options} [_options]
   * @return {Promise.<object>}
   */
  async updateOne(params = {}, data = {}, _restrictions = {}, _options = {}) {
    const entity = await this.model.findOne({
      where: params,
    });
    if (entity) {
      await entity.update(data);
    }
    return entity;
  }

  /**
   * @public
   * @param {object} [params]
   * @param {object} [data]
   * @param {Layers.AbstractModelsLayer.restrictions} [_restrictions]
   * @param {Layers.AbstractModelsLayer.options} [_options]
   * @return {boolean}
   */
  async updateAll(params = {}, data = {}, _restrictions = {}, _options = {}) {
    return !!(await this.model.update(
      data,
      { where: params },
    ));
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.filter} filter
   * @param {Layers.AbstractModelsLayer.pagination} [pagination]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @param {Layers.AbstractModelsLayer.options} [options]
   * @return {Promise<void>}
   */
  async getAll(filter = {}, pagination = {}, restrictions = {}, options = {}) {
    const where = {};

    this.applyRestrictions(where, restrictions);
    this.applyFilter(where, filter);

    const query = {
      limit: pagination.limit,
      offset: pagination.offset,
      where,
    };

    return this.model.findAll(query, options);
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   * @param {Layers.AbstractModelsLayer.pagination} [pagination]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @param {Layers.AbstractModelsLayer.options} [_options]
   * @return {Promise<{data: Models.Model[], total: number}>}
   */
  async getAllWithConnections(filter = {}, pagination = {}, restrictions = {}, _options = {}) {
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
   * @public
   * @param {object} [params]
   * @param {object} [query]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @param {Layers.AbstractModelsLayer.options} [_options]
   * @return {Promise<Models.Model>}
   */
  async getOne(params = {}, query = {}, restrictions = {}, _options = {}) {
    const where = { ...params };
    const sort = query.sort === 1 ? 'ASC' : 'DESC';
    const order = [['createdAt', sort]];

    this.applyRestrictions(where, restrictions);

    return this.model.findOne({
      where,
      order,
    });
  }

  /**
   * @public
   * @param {object} params
   * @param {object} query
   * @param {object} data
   * @param {Layers.AbstractModelsLayer.restrictions} restrictions
   * @param {Layers.AbstractModelsLayer.options} options
   * @return {Promise<Models.Model>}
   */
  async upsertOne(params, query, data, restrictions, options) {
    return await this.getOne(params, query, restrictions, options)
      || await this.createOne(data, restrictions, options);
  }

  /**
   * @public
   * @return {Promise<*>}
   */
  async getTransaction() {
    return this.sequelize.transaction();
  }

  /**
   * @public
   * @param {object} transaction
   * @return {Promise<*>}
   */
  async commit(transaction) {
    return transaction.commit();
  }
}

module.exports = {
  ModelPostgres,
};
