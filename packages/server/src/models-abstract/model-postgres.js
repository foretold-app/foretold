const _ = require('lodash');
const { Op } = require('sequelize');

const models = require('../models');

const { Model } = require('./model');
const { ResponseAll } = require('./classes/response-all');

const { splitBy } = require('../lib/functions');

/**
 * @abstract
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
    this.or = Op.or;

    this.fn = this.sequelize.fn;
    this.col = this.sequelize.col;
    this.literal = this.sequelize.literal;
  }

  /**
   * @protected
   * @todo: see this._channelIds()
   * @param {Models.ObjectID} [agentId]
   * @param {string} [name]
   * @return {Sequelize.literal}
   */
  _channelIdsLiteral(agentId, name = '') {
    return this.literal(this._channelIds(agentId, name));
  }

  /**
   * @protected
   * @param {Models.ObjectID} [agentId]
   * @param {string} [name]
   * @return {string}
   */
  _channelIds(agentId, name = '') {
    return `(
      /* P͟u͟b͟l͟i͟c͟ ͟a͟n͟d͟ ͟J͟o͟i͟n͟e͟d͟ ͟C͟h͟a͟n͟n͟e͟l͟s͟ (${name}) */
      SELECT "Channels"."id" FROM "Channels"
      LEFT OUTER JOIN 
        "ChannelMemberships" 
        ON "Channels".id = "ChannelMemberships"."channelId"
        AND "ChannelMemberships"."agentId" = '${agentId}'
      WHERE 
        "ChannelMemberships"."agentId" IS NOT NULL
        OR "Channels"."isPublic" = TRUE 
    )`;
  }

  /**
   * @protected
   * @para {string} [name]
   * @return {Sequelize.literal}
   */
  _channelIdsPublicLiteral(name = '') {
    return this.literal(this._channelIdsPublic(name));
  }

  /**
   * @protected
   * @param {string} [name]
   * @return {string}
   */
  _channelIdsPublic(name = '') {
    return `(
      /* P͟u͟b͟l͟i͟c͟ ͟C͟h͟a͟n͟n͟e͟l͟s͟ (${name}) */
      SELECT "Channels"."id" FROM "Channels"
      WHERE "Channels"."isPublic" = TRUE
    )`;
  }

  /**
   * @todo: Rename to "withinJoinedChannels" + "Literal"?
   * @protected
   * @param {Models.ObjectID} [agentId]
   * @param {string} [name]
   * @return {Sequelize.literal}
   */
  _channelIdsByMembersLiteral(agentId, name = '') {
    return this.literal(this._channelIdsByMembers(agentId, name));
  }

  /**
   * @todo: Rename to "withinJoinedChannels"?
   * @protected
   * @param {Models.ObjectID} [agentId]
   * @param {string} [name]
   * @return {string}
   */
  _channelIdsByMembers(agentId, name = '') {
    return `(
      /* J͟o͟i͟n͟e͟d͟ ͟C͟h͟a͟n͟n͟e͟l͟s͟ (${name}) */
      SELECT "Channels"."id" FROM "Channels"
      LEFT OUTER JOIN 
        "ChannelMemberships" 
        ON "Channels".id = "ChannelMemberships"."channelId"
        AND "ChannelMemberships"."agentId" = '${agentId}'
      WHERE 
        "ChannelMemberships"."agentId" IS NOT NULL
    )`;
  }

  /**
   * @protected
   * @param {Models.ObjectID} [agentId]
   * @param {string} [name]
   * @return {Sequelize.literal}
   */
  _measurableIdsLiteral(agentId, name = '') {
    return this.literal(this._measurableIds(agentId, name));
  }

  /**
   * @protected
   * @param {string} [agentId]
   * @param {string} [name]
   * @return {string}
   */
  _measurableIds(agentId, name = '') {
    return `(
      /* Description (${name}) */
      WITH channelIds AS (${this._channelIds(agentId, name)})
      SELECT "Measurables"."id" FROM "Measurables"
      WHERE "Measurables"."channelId" IN (SELECT id FROM channelIds)
    )`;
  }

  /**
   * @todo: To add mind map from Params to DB query.
   * @protected
   * @param {object} [where]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @return {object}
   */
  applyRestrictions(where = {}, restrictions = {}) {
    if (restrictions.isAdmin) return where;
    if (!where[this.and]) where[this.and] = [];

    this.applyAbstracts(where, restrictions);

    // @todo: Use "withinPublicChannels" as "channelId"
    if (restrictions.channelId && !restrictions.agentId) {
      where[this.and].push({
        channelId: {
          [this.in]: this._channelIdsPublicLiteral('Restrictions'),
        },
      });
    }

    // @todo: Use "withinPublicAndJoinedChannels" as "channelId"
    if (restrictions.channelId && restrictions.agentId) {
      where[this.and].push({
        channelId: {
          [this.in]: this._channelIdsLiteral(restrictions.agentId, 'Restrictions'),
        },
      });
    }

    // @todo: Use "withinPublicAndJoinedChannels" as "id"
    if (restrictions.channelIdAsId && restrictions.agentId) {
      where[this.and].push({
        id: {
          [this.in]: this._channelIdsLiteral(restrictions.agentId, 'Restrictions'),
        },
      });
    }

    if (restrictions.userId) {
      where[this.and].push({
        userId: restrictions.userId,
      });
    }

    if (restrictions.measurableId && restrictions.agentId) {
      where[this.and].push({
        measurableId: {
          [this.in]: this._measurableIdsLiteral(restrictions.agentId, 'Restrictions'),
        },
      });
    }

    return where;
  }

  /**
   * @protected
   * @param {object} [include]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @return {*}
   */
  applyRestrictionsIncluding(include = [], restrictions = {}) {
    if (!include) include = [];

    // @todo: It is a filter, b͟u͟t͟ ͟n͟o͟t͟ ͟r͟e͟s͟t͟r͟i͟c͟t͟i͟o͟n͟
    // @todo: Use object structures.
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
   * Extend this method in child classes.
   * @param {object} [where]
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   * @param {Models.ObjectID} [filter.userId]
   */
  applyFilter(where = {}, filter = {}) {
    if (!where) where = {};
    if (!where[this.and]) where[this.and] = [];

    this.applyAbstracts(where, filter);

    // OK
    if (filter.isArchived) {
      where[this.and].push({
        isArchived: {
          [this.in]: this._getBooleansOfList(filter.isArchived),
        }
      });
    }

    // OK
    if (filter.userId) {
      where[this.and].push({
        userId: filter.userId,
      });
    }

    // OK
    if (filter.agentId) {
      where[this.and].push({
        agentId: filter.agentId,
      });
    }

    // OK
    if (filter.channelId) {
      where[this.and].push({
        channelId: filter.channelId,
      });
    }

    // OK?
    const startDate = _.get(filter, 'findInDateRange.startDate');
    const endDate = _.get(filter, 'findInDateRange.endDate');

    if (filter.measurableId) where.measurableId = filter.measurableId;
    if (filter.competitorType) where.competitorType = {
      [this.in]: filter.competitorType,
    };
    if (startDate) where[this.and].push({
      createdAt: { [this.gte]: startDate },
    });
    if (endDate) where[this.and].push({ createdAt: { [this.lte]: endDate } });
    if (filter.notTaggedByAgent) where.id = {
      [this.notIn]: this._taggedMeasurementsLiteral(filter.notTaggedByAgent),
    };

    return where;
  }

  /**
   * @protected
   * Extend this method in child classes.
   * @param {object} [where]
   * @param {Layers.AbstractModelsLayer.filter
   * | Layers.AbstractModelsLayer.restrictions} [abstractions]
   */
  applyAbstracts(where = {}, abstractions = {}) {
    if (!where) where = {};
    if (!where[this.and]) where[this.and] = [];

    const name = _.get(abstractions, 'constructor.name', 'Abstraction');

    // OK
    if (abstractions.withinJoinedChannels) {
      const { as, agentId } = abstractions.withinJoinedChannels;
      where[this.and].push({
        [as]: {
          [this.in]: this._channelIdsByMembersLiteral(agentId, name),
        },
      });
    }

    // OK
    if (abstractions.withinPublicAndJoinedChannels) {
      const { as, agentId } = abstractions.withinPublicAndJoinedChannels;
      where[this.and].push({
        [as]: {
          [this.in]: this._channelIdsLiteral(agentId, name),
        },
      });
    }

    return where;
  }

  /**
   * @protected
   * @param {Layers.AbstractModelsLayer.pagination} [pagination]
   * @param {number} total
   * @return {{offset: number, limit: number }}
   */
  _getPagination(pagination = {}, total = 0) {
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
  _setIndexes(data = [], edgePagination = {}) {
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
  _getBooleansOfList(list = []) {
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
   * @param {Layers.AbstractModelsLayer.options} [options]
   * @return {Promise.<object>}
   */
  async createOne(data = {}, _restrictions = {}, options = {}) {
    return this.model.create(data, options);
  }

  /**
   * @public
   * @param {object} [params]
   * @param {object} [data]
   * @param {Layers.AbstractModelsLayer.restrictions} [_restrictions]
   * @param {Layers.AbstractModelsLayer.options} [options]
   * @return {Promise.<object>}
   */
  async updateOne(params = {}, data = {}, _restrictions = {}, options = {}) {
    const findCond = { where: { ...params } };
    const updateCond = {};
    this._extendConditions(findCond, options);
    this._extendConditions(updateCond, options);

    const entity = await this.model.findOne(findCond);
    if (entity) await entity.update(data, updateCond);
    return entity;
  }

  /**
   * @public
   * @param {object} [params]
   * @param {object} [data]
   * @param {Layers.AbstractModelsLayer.restrictions} [_restrictions]
   * @param {Layers.AbstractModelsLayer.options} [options]
   * @return {boolean}
   */
  async updateAll(params = {}, data = {}, _restrictions = {}, options = {}) {
    const cond = { where: { ...params } };
    this._extendConditions(cond, options);
    return !!(await this.model.update(data, cond));
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.filter} filter
   * @param {Layers.AbstractModelsLayer.pagination} [pagination]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @param {Layers.AbstractModelsLayer.options} [options]
   * @return {Promise<*[]>}
   */
  async getAll(filter = {}, pagination = {}, restrictions = {}, options = {}) {
    const where = {};

    if ('inspect' in filter) filter.inspect();
    if ('inspect' in pagination) pagination.inspect();
    if ('inspect' in restrictions) restrictions.inspect();

    this.applyRestrictions(where, restrictions);
    this.applyFilter(where, filter);

    const cond = {
      limit: pagination.limit,
      offset: pagination.offset,
      where,
    };

    this._extendConditions(cond, options);

    return this.model.findAll(cond);
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   * @param {Layers.AbstractModelsLayer.pagination} [pagination]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @param {Layers.AbstractModelsLayer.options} [options]
   * @return {Promise<{data: Models.Model[], total: number}>}
   */
  async getAllWithConnections(
    filter = {},
    pagination = {},
    restrictions = {},
    options = {},
  ) {
    const where = {};
    const include = [];

    if ('inspect' in filter) filter.inspect();
    if ('inspect' in pagination) pagination.inspect();
    if ('inspect' in restrictions) restrictions.inspect();

    this.applyRestrictions(where, restrictions);
    this.applyRestrictionsIncluding(include, restrictions);
    this.applyFilter(where, filter);

    const cond = { where, include };
    this._extendConditions(cond, options);

    /** @type {number} */
    const total = await this.model.count(cond);
    const edgePagination = this._getPagination(pagination, total);

    const findCond = {
      ...cond,
      limit: edgePagination.limit,
      offset: edgePagination.offset,
      order: pagination.order,
    };

    /** @type {Models.Model[]} */
    let data = await this.model.findAll(findCond);
    data = this._setIndexes(data, edgePagination);
    data.total = total;

    // @todo: tricky, rework
    const spacedLimit = _.get(filter, 'findInDateRange.spacedLimit');
    if (spacedLimit) data = splitBy(data, spacedLimit);

    return new ResponseAll(data, total);
  }

  /**
   * @public
   * @param {object} [params]
   * @param {object} [query]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @param {Layers.AbstractModelsLayer.options} [options]
   * @return {Promise<Models.Model>}
   */
  async getOne(params = {}, query = {}, restrictions = {}, options = {}) {
    const where = { ...params };
    const sort = query.sort === 1 ? 'ASC' : 'DESC';
    const order = [['createdAt', sort]];

    this.applyRestrictions(where, restrictions);

    const cond = {
      where,
      order,
    };

    this._extendConditions(cond, options);

    return this.model.findOne(cond);
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

  /**
   * @public
   * @param {object} transaction
   * @return {Promise<*>}
   */
  async rollback(transaction) {
    return transaction.rollback();
  }

  /**
   * @protected
   * @param {object} cond
   * @param {Layers.AbstractModelsLayer.options} options
   */
  _extendConditions(cond = {}, options = {}) {
    if (_.has(options, 'transaction')) {
      cond.transaction = options.transaction;
    }
    if (_.has(options, 'lock')) {
      cond.lock = options.lock;
    }
    if (_.has(options, 'skipLocked')) {
      cond.skipLocked = options.skipLocked;
    }
    return cond;
  }
}

module.exports = {
  ModelPostgres,
};
