const _ = require('lodash');
const assert = require('assert');
const { Op } = require('sequelize');

const models = require('./definitions');
const logger = require('../../lib/log');

const { Model } = require('./model');

const { Options } = require('./classes');
const { Restrictions } = require('./classes');
const { ResponseAll } = require('./classes');

const { Data } = require('../classes');
const { Filter } = require('../classes');
const { Pagination } = require('../classes');
const { Params } = require('../classes');
const { Query } = require('../classes');

/**
 * @abstract
 * @implements {Layers.Models.ModelGeneric}
 */
class ModelPostgres extends Model {
  constructor(
    {
      model,
      sequelize,
    },
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

    this.log = logger.module('models/model-postgres');
  }

  /**
   * @public
   * @param {Layers.Models.Data} [data]
   * @param {Layers.Models.ModelRestrictions} [restrictions]
   * @param {Layers.Models.ModelOptions} [options]
   * @return {Promise.<object>}
   */
  async createOne(
    data = new Data(),
    restrictions = new Restrictions(),
    options = new Options(),
  ) {
    this._assertInput({ data, restrictions, options });
    return this.model.create(data, options);
  }

  /**
   * @public
   * @param {Layers.Params} [params]
   * @param {Layers.Models.Data} [data]
   * @param {Layers.Models.ModelRestrictions} [restrictions]
   * @param {Layers.Models.ModelOptions} [options]
   * @return {Promise.<object>}
   */
  async updateOne(
    params = new Params(),
    data = new Data(),
    restrictions = new Restrictions(),
    options = new Options(),
  ) {
    this._assertInput({ params, data, restrictions, options });

    const cond = { where: { ...params } };
    this._extendGenericConditions(cond, options);

    const entity = await this.model.findOne(cond);
    if (!!entity) await this.model.update(data, cond);
    return entity;
  }

  /**
   * @public
   * @param {Layers.Params} [params]
   * @param {Layers.Models.Data} [data]
   * @param {Layers.Models.ModelRestrictions} [restrictions]
   * @param {Layers.Models.ModelOptions} [options]
   * @return {boolean}
   */
  async updateAll(
    params = new Params(),
    data = new Data(),
    restrictions = new Restrictions(),
    options = new Options(),
  ) {
    this._assertInput({ params, data, restrictions, options });
    const cond = { where: { ...params } };
    this._extendGenericConditions(cond, options);
    return !!(await this.model.update(data, cond));
  }

  /**
   * @public
   * @param {Layers.Filter} filter
   * @param {Layers.Pagination} [pagination]
   * @param {Layers.Models.ModelRestrictions} [restrictions]
   * @param {Layers.Models.ModelOptions} [options]
   * @return {Promise<*[]>}
   */
  async getAll(
    filter = new Filter(),
    pagination = new Pagination(),
    restrictions = new Restrictions(),
    options = new Options(),
  ) {
    // Block 1
    this._assertInput({ filter, pagination, restrictions, options });
    const where = {};

    // if ('inspect' in filter) filter.inspect();
    // if ('inspect' in pagination) pagination.inspect();
    // if ('inspect' in restrictions) restrictions.inspect();

    this.applyRestrictions(where, restrictions);
    this.applyFilter(where, filter);

    // Block 2
    const order = pagination.isOrderSet()
      ? this._getOrderFromPagination(pagination)
      : this._getDefaultOrder();

    const findCond = {
      limit: pagination.limit,
      offset: pagination.offset,
      where,
      order,
    };
    this._extendGenericConditions(findCond, options);
    this._extendAdvancedConditions(findCond, options);
    const data = await this.model.findAll(findCond);

    return data;
  }

  /**
   * @todo: To use an order with attributes for the measurables.
   * @public
   * @param {Layers.Filter} [filter]
   * @param {Layers.Pagination} [pagination]
   * @param {Layers.Models.ModelRestrictions} [restrictions]
   * @param {Layers.Models.ModelOptions} [options]
   * @return {Promise<{data: Models.Definition[], total: number}>}
   */
  async getAllWithConnections(
    filter = new Filter(),
    pagination = new Pagination(),
    restrictions = new Restrictions(),
    options = new Options(),
  ) {
    // Block 1
    this._assertInput({ filter, pagination, restrictions, options });
    const where = {};
    const include = [];

    // if ('inspect' in filter) filter.inspect();
    // if ('inspect' in pagination) pagination.inspect();
    // if ('inspect' in restrictions) restrictions.inspect();

    this.applyRestrictions(where, restrictions);
    this.applyFilter(where, filter);
    this.applyFilterIncluding(include, filter);

    // Block 2
    const { limit, offset } = pagination.getPagination();
    const order = pagination.isOrderSet()
      ? this._getOrderFromPagination(pagination)
      : this._getDefaultOrder();

    const findCond = {
      where,
      include,
      limit,
      offset,
      order,
    };
    this._extendGenericConditions(findCond, options);
    this._extendAdvancedConditions(findCond, options);
    const data = await this.model.findAll(findCond);

    // Block 3
    const countCond = { where, include };
    this._extendGenericConditions(countCond, options);
    const total = await this.model.count(countCond);

    // Block 4
    const spacedLimit = filter.getSpacedLimit();
    return new ResponseAll(data, total, offset, spacedLimit);
  }

  /**
   * @public
   * @param {Layers.Params} [params]
   * @param {Layers.Query} [query]
   * @param {Layers.Models.ModelRestrictions} [restrictions]
   * @param {Layers.Models.ModelOptions} [options]
   * @return {Promise<Models.Definition>}
   */
  async getOne(
    params = new Params(),
    query = new Query(),
    restrictions = new Restrictions(),
    options = new Options(),
  ) {
    this._assertInput({ params, query, restrictions, options });
    const where = { ...params };

    // if ('inspect' in params) params.inspect();
    // if ('inspect' in restrictions) restrictions.inspect();

    this.applyRestrictions(where, restrictions);

    const order = this._getOrderForOne(query);
    const distinct = !!query.distinct ? true : null;
    const col = !!query.col ? query.col : null;

    const cond = {
      where,
      order,
      distinct,
      col,
    };
    this._extendGenericConditions(cond, options);
    this._extendAdvancedConditions(cond, options);

    return this.model.findOne(cond);
  }

  /**
   * @todo: To fix a "spread".
   * @public
   * @param {Layers.Params} [params]
   * @param {Layers.Query} [query]
   * @param {Layers.Models.ModelRestrictions} [restrictions]
   * @param {Layers.Models.ModelOptions} [options]
   * @return {Promise<Models.Definition>}
   */
  async getCount(
    params = new Params(),
    query = new Query(),
    restrictions = new Restrictions(),
    options = new Options(),
  ) {
    this._assertInput({ params, query, restrictions, options });
    const where = { ...params };

    // if ('inspect' in params) params.inspect();
    // if ('inspect' in restrictions) restrictions.inspect();

    this.applyRestrictions(where, restrictions);

    const distinct = !!query.distinct ? true : null;
    const col = !!query.col ? query.col : null;

    const cond = {
      where,
      distinct,
      col,
    };
    this._extendGenericConditions(cond, options);
    this._extendAdvancedConditions(cond, options);

    return this.model.count(cond);
  }

  /**
   * @public
   * @param {Layers.Params} [params]
   * @param {Layers.Query} [query]
   * @param {Layers.Models.Data} data
   * @param {Layers.Models.ModelRestrictions} restrictions
   * @param {Layers.Models.ModelOptions} options
   * @return {Promise<Models.Definition>}
   */
  async upsertOne(params, query, data, restrictions, options) {
    return await this.getOne(params, query, restrictions, options)
      || this.createOne(data, restrictions, options);
  }

  /**
   * @todo: To fix a "spread".
   * @public
   * @param {Layers.Params} [params]
   * @param {Layers.Query} [query]
   * @param {Layers.Models.ModelRestrictions} restrictions
   * @param {Layers.Models.ModelOptions} options
   * @return {Promise<Models.Definition>}
   */
  async deleteOne(
    params = new Params(),
    query = new Query(),
    restrictions = new Restrictions(),
    options = new Options(),
  ) {
    this._assertInput({ params, query, restrictions, options });
    const entity = await this.getOne(params, query, restrictions, options);
    if (entity) {
      const where = { ...params };
      const cond = { where };
      this._extendGenericConditions(cond, options);
      await this.model.destroy(cond);
    }
    return entity;
  }

  /**
   * @public
   * @return {Promise<Layers.Transaction>}
   */
  async getTransaction() {
    return this.sequelize.transaction();
  }

  /**
   * @public
   * @param {Layers.Transaction} transaction
   * @return {Promise<*>}
   */
  async commit(transaction) {
    return transaction.commit();
  }

  /**
   * @public
   * @param {Layers.Transaction} transaction
   * @return {Promise<*>}
   */
  async rollback(transaction) {
    return transaction.rollback();
  }

  /**
   * @todo: To add mind map from Params to DB query.
   * @protected
   * @param {object} [where]
   * @param {Layers.Models.ModelRestrictions} [restrictions]
   * @return {object}
   */
  applyRestrictions(where = {}, restrictions = new Restrictions()) {
    if (restrictions.isAdmin) return where;
    if (!where[this.and]) where[this.and] = [];

    this.applyAbstracts(where, restrictions);

    // @todo: Use "withinPublicChannels" as "channelId"
    if (!!restrictions.channelId && !restrictions.agentId) {
      where[this.and].push({
        channelId: {
          [this.in]: this._publicChannelsLiteral('Restrictions'),
        },
      });
    }

    // @todo: Use "withinPublicAndJoinedChannels" as "channelId"
    if (!!restrictions.channelId && !!restrictions.agentId) {
      where[this.and].push({
        channelId: {
          [this.in]: this._publicAndJoinedChannelsLiteral(
            restrictions.agentId,
            'Restrictions',
          ),
        },
      });
    }

    // @todo: Use "withinPublicAndJoinedChannels" as "id"
    if (!!restrictions.channelIdAsId && !!restrictions.agentId) {
      where[this.and].push({
        id: {
          [this.in]: this._publicAndJoinedChannelsLiteral(
            restrictions.agentId,
            'Restrictions',
          ),
        },
      });
    }

    // @todo: Use "withinPublicAndJoinedChannels" as "id"
    if (!!restrictions.channelIdAsId && !restrictions.agentId) {
      where[this.and].push({
        id: {
          [this.in]: this._publicChannelsLiteral('Restrictions'),
        },
      });
    }

    if (!!restrictions.userId) {
      where[this.and].push({
        userId: restrictions.userId,
      });
    }

    // @todo: Use structures.
    if (!!restrictions.measurableId && !!restrictions.agentId) {
      where[this.and].push({
        measurableId: {
          [this.in]: this._measurablesInPublicAndJoinedChannelsLiteral(
            restrictions.agentId,
            'Restrictions',
          ),
        },
      });
    }

    // @todo: Use structures.
    if (!!restrictions.measurableId && !restrictions.agentId) {
      where[this.and].push({
        measurableId: {
          [this.in]: this._measurablesInPublicChannelsLiteral(
            'Restrictions',
          ),
        },
      });
    }

    return where;
  }

  /**
   * @protected
   * @todo: Try to fix it and do not use "this.models.Measurement".
   * @param {object} [include]
   * @param {Layers.Filter} [filter]
   * @return {*}
   */
  applyFilterIncluding(include = [], filter = new Filter()) {
    if (!include) include = [];

    // @todo: To rework.
    // @todo: Use object structures.
    if (!!filter.measuredByAgentId) {
      include.push({
        model: this.models.Measurement,
        as: 'Measurements',
        where: { agentId: filter.measuredByAgentId },
      });
    }

    return include;
  }

  /**
   * If you define filters in the generic type Filter
   * then write code only in this parent method.
   * Do not place pieces of code in child classes.
   * Since all child classes should have access to these
   * filters.
   *
   * @protected
   * Extend this method in child classes.
   * @param {object} [where]
   * @param {Layers.Filter} [filter]
   * @param {Defs.AgentID} [filter.userId]
   */
  applyFilter(where = {}, filter = new Filter()) {
    if (!where) where = {};
    if (!where[this.and]) where[this.and] = [];

    const name = _.get(filter, 'constructor.name', 'Filter');
    this.applyAbstracts(where, filter);

    if (!!filter.labelProperty) {
      where[this.and].push({
        labelProperty: {
          [this.in]: filter.labelProperty,
        },
      });
    }

    if (!!filter.labelSubject) {
      where[this.and].push({
        labelSubject: {
          [this.in]: filter.labelSubject,
        },
      });
    }

    if (!!filter.isArchived) {
      where[this.and].push({
        isArchived: {
          [this.in]: this._getBooleansOfList(filter.isArchived),
        },
      });
    }

    if (!!filter.measurableIds) {
      where[this.and].push({
        id: {
          [this.in]: filter.measurableIds,
        },
      });
    }

    if (!!filter.role) {
      where[this.and].push({
        role: filter.role,
      });
    }

    if (!!filter.userId) {
      where[this.and].push({
        userId: filter.userId,
      });
    }

    if (!!filter.taggedMeasurementId) {
      where[this.and].push({
        taggedMeasurementId: filter.taggedMeasurementId,
      });
    }

    if (!!filter.agentId) {
      where[this.and].push({
        agentId: filter.agentId,
      });
    }

    if (!!filter.channelId) {
      where[this.and].push({
        channelId: filter.channelId,
      });
    }

    if (!!filter.measurableId) {
      where[this.and].push({
        measurableId: filter.measurableId,
      });
    }

    if (!!filter.measurementId) {
      where[this.and].push({
        measurementId: filter.measurementId,
      });
    }

    if (!!filter.email) {
      where[this.and].push({
        email: filter.email,
      });
    }

    if (!!filter.status) {
      where[this.and].push({
        status: filter.status,
      });
    }

    if (!!filter.competitorType) {
      where[this.and].push({
        competitorType: {
          [this.in]: filter.competitorType,
        },
      });
    }

    if (!!filter.notTaggedByAgent) {
      where[this.and].push({
        id: {
          [this.notIn]: this._taggedMeasurementsLiteral(
            filter.notTaggedByAgent,
            name,
          ),
        },
      });
    }

    if (!!filter.isVerified) {
      where[this.and].push({
        agentId: {
          [this.in]: this._verifiedMeasurementsLiteral(name),
        },
      });
    }

    const startDate = _.get(filter, 'findInDateRange.startDate');
    if (!!startDate) {
      where[this.and].push({
        createdAt: { [this.gte]: startDate },
      });
    }

    const endDate = _.get(filter, 'findInDateRange.endDate');
    if (!!endDate) {
      where[this.and].push({
        createdAt: { [this.lte]: endDate },
      });
    }

    if (_.isArray(filter.states)) {
      where[this.and].push({
        state: { [this.in]: filter.states },
      });
    }

    if (!!filter.seriesId) {
      where[this.and].push({
        seriesId: filter.seriesId,
      });
    }

    if (!!filter.ownerId) {
      where[this.and].push({
        ownerId: filter.ownerId,
      });
    }

    if (!!filter.creatorId) {
      where[this.and].push({
        creatorId: filter.creatorId,
      });
    }

    if (!!filter.isEmailVerified) {
      where[this.and].push({
        isEmailVerified: {
          [this.in]: filter.isEmailVerified,
        },
      });
    }

    if (!!filter.isNotEmailVerified) {
      where[this.and].push({
        [this.or]: [
          { isEmailVerified: false },
          { isEmailVerified: null },
        ],
      });
    }

    if (!!filter.notAuth0AccessToken) {
      where[this.and].push({
        auth0AccessToken: {
          [this.not]: null,
        },
      });
    }

    if (!!filter.excludeChannelId) {
      where[this.and].push({
        id: {
          [this.notIn]: this._agentsIdsLiteral(filter.excludeChannelId, name),
        },
      });
    }

    if (!!filter.types) {
      where[this.and].push({
        type: {
          [this.in]: filter.types,
        },
      });
    }

    if (filter.sentAt || filter.sentAt === null) {
      where[this.and].push({
        sentAt: filter.sentAt,
      });
    }

    if (!!filter.notificationId) {
      where[this.and].push({
        notificationId: filter.notificationId,
      });
    }

    if (_.isNumber(filter.attemptCounterMax)) {
      where[this.and].push({
        attemptCounter: {
          [this.lt]: filter.attemptCounterMax,
        },
      });
    }

    if (_.isNumber(filter.minPredictionCountTotal)) {
      where[this.and].push({
        predictionCountTotal: {
          [this.gte]: filter.minPredictionCountTotal,
        },
      });
    }

    if (_.isNumber(filter.minNumberOfPredictions)) {
      where[this.and].push({
        numberOfPredictions: {
          [this.gte]: filter.minNumberOfPredictions,
        },
      });
    }

    if (_.isNumber(filter.minNumberOfQuestionsScored)) {
      where[this.and].push({
        numberOfQuestionsScored: {
          [this.gte]: filter.minNumberOfQuestionsScored,
        },
      });
    }

    return where;
  }

  /**
   * @protected
   * Extend this method in child classes.
   * @param {object} [where]
   * @param {Layers.Filter
   * | Layers.Models.ModelRestrictions} [abstractions]
   */
  applyAbstracts(where = {}, abstractions = {}) {
    if (!where) where = {};
    if (!where[this.and]) where[this.and] = [];

    const name = _.get(abstractions, 'constructor.name', 'Abstraction');

    if (!!abstractions.withinPublicChannels) {
      const { as } = abstractions.withinPublicChannels;
      where[this.and].push({
        [as]: {
          [this.in]: this._publicChannelsLiteral(name),
        },
      });
    }

    if (!!abstractions.withinJoinedChannels) {
      const { as, agentId } = abstractions.withinJoinedChannels;
      where[this.and].push({
        [as]: {
          [this.in]: this._joinedChannelsLiteral(agentId, name),
        },
      });
    }

    if (!!abstractions.withinPublicAndJoinedChannels) {
      const { as, agentId } = abstractions.withinPublicAndJoinedChannels;
      where[this.and].push({
        [as]: {
          [this.in]: this._publicAndJoinedChannelsLiteral(agentId, name),
        },
      });
    }

    if (!!abstractions.withinMeasurables) {
      const { as, states, channelId } = abstractions.withinMeasurables;
      where[this.and].push({
        [as]: {
          [this.in]: this._withinMeasurablesLiteral(states, channelId, name),
        },
      });
    }

    return where;
  }

  /**
   * @protected
   * @todo: see this._publicAndJoinedChannels()
   * @param {Defs.AgentID} [agentId]
   * @param {string} [name]
   * @return {Sequelize.literal}
   */
  _publicAndJoinedChannelsLiteral(agentId, name = '') {
    return this.literal(this._publicAndJoinedChannels(agentId, name));
  }

  /**
   * @todo: sequelize.dialect.QueryGenerator.selectQuery
   * @todo: To use SQL replacements or bindings.
   * @protected
   * @param {Defs.AgentID} [agentId]
   * @param {string} [name]
   * @return {string}
   */
  _publicAndJoinedChannels(agentId, name = '') {
    assert(!!agentId, 'Agent ID is required.');
    return `(
      /* Public and Joined Channels (${name}) */
      SELECT "Channels"."id" FROM "Channels"
      LEFT OUTER JOIN "ChannelMemberships"
        ON "Channels".id = "ChannelMemberships"."channelId"
        AND (
          "ChannelMemberships"."agentId" = '${agentId}'
          OR "ChannelMemberships"."agentId" = (
            /* AgentId of bot owner */
            SELECT "Users"."agentId" FROM "Bots"
            LEFT JOIN "Users" ON "Users"."id" = "Bots"."userId"
            WHERE "Bots"."agentId" = '${agentId}'
            LIMIT 1
          )
        )
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
  _publicChannelsLiteral(name = '') {
    return this.literal(this._publicChannels(name));
  }

  /**
   * @protected
   * @param {string} [name]
   * @return {string}
   */
  _publicChannels(name = '') {
    return `(
      /* Public Channels (${name}) */
      SELECT "Channels"."id" FROM "Channels"
      WHERE "Channels"."isPublic" = TRUE
    )`;
  }

  /**
   * @todo: Rename to "withinJoinedChannels" + "Literal"?
   * @protected
   * @param {Defs.AgentID} [agentId]
   * @param {string} [name]
   * @return {Sequelize.literal}
   */
  _joinedChannelsLiteral(agentId, name = '') {
    return this.literal(this._joinedChannels(agentId, name));
  }

  /**
   * @todo: To use SQL replacements or bindings.
   * @todo: Rename to "withinJoinedChannels"?
   * @protected
   * @param {Defs.AgentID} [agentId]
   * @param {string} [name]
   * @return {string}
   */
  _joinedChannels(agentId, name = '') {
    assert(!!agentId, 'Agent ID is required.');
    return `(
      /* Joined Channel (${name}) */
      SELECT "Channels"."id" FROM "Channels"
      LEFT OUTER JOIN "ChannelMemberships"
        ON "Channels".id = "ChannelMemberships"."channelId"
        AND "ChannelMemberships"."agentId" = '${agentId}'
      WHERE
        "ChannelMemberships"."agentId" IS NOT NULL
    )`;
  }


  /**
   * @todo: see this._publicAndJoinedChannels()
   * @protected
   * @param {Defs.ChannelID} channelId
   * @param {string} name
   * @return {Sequelize.literal}
   */
  _agentsIdsLiteral(channelId, name = '') {
    return this.literal(this._agentsIds(channelId, name));
  }

  /**
   * @todo: Use ORM opportunities to join tables.
   * @todo: No, do not this ORM for this.
   * @protected
   * @param {Defs.ChannelID} channelId
   * @param {string} name
   * @return {string}
   */
  _agentsIds(channelId, name = '') {
    assert(!!channelId, 'Channel ID is required.');
    return `(
      /* AgentIds (${name}) */
      SELECT "ChannelMemberships"."agentId" FROM "ChannelMemberships"
      WHERE "ChannelMemberships"."channelId" = '${channelId}'
    )`;
  }

  /**
   * @param {string} [name]
   * @return {string}
   */
  _verifiedMeasurementsLiteral(name = '') {
    return this.literal(this._verifiedMeasurements(name));
  }

  /**
   * @protected
   * @param {string} name
   * @return {string}
   */
  _verifiedMeasurements(name = '') {
    return `(
      /* Verified Channel Memberships (${name}) */
      WITH "Measurable" AS (
          SELECT "Measurables"."channelId"
          FROM "Measurables"
          WHERE "Measurables"."id" = "Measurement"."measurableId"
          LIMIT 1
      ),
      "Channel" AS (
          SELECT "Channels"."requireVerification", "Channels"."id"
          FROM "Channels", "Measurable"
          WHERE "Channels"."id" = "Measurable"."channelId"
          LIMIT 1
      )
      SELECT "ChannelMemberships"."agentId"
      FROM "ChannelMemberships", "Channel"
      WHERE "ChannelMemberships"."channelId" = "Channel"."id"
        AND ("ChannelMemberships"."isVerified" = TRUE
          OR "Channel"."requireVerification" != TRUE)
    )`;
  }

  /**
   * @param {Defs.AgentID} agentId
   * @param {string} [name]
   * @return {string}
   */
  _taggedMeasurementsLiteral(agentId, name = '') {
    return this.literal(this._taggedMeasurements(agentId, name));
  }

  /**
   * @protected
   * @param {Defs.AgentID} agentId
   * @param {string} name
   * @return {string}
   */
  _taggedMeasurements(agentId, name = '') {
    assert(!!agentId, 'Agent ID is required.');
    return `(
      /* Tagged Measurements͟ (${name}) */
      SELECT "taggedMeasurementId"
      FROM "Measurements"
      WHERE "agentId" = '${agentId}'
      AND "taggedMeasurementId" IS NOT NULL
    )`;
  }

  /**
   * @protected
   * @param {Defs.AgentID} [agentId]
   * @param {string} [name]
   * @return {Sequelize.literal}
   */
  _measurablesInPublicAndJoinedChannelsLiteral(agentId, name = '') {
    return this.literal(
      this._measurablesInPublicAndJoinedChannels(agentId, name),
    );
  }

  /**
   * @todo: To use SQL replacements or bindings.
   * @param {Defs.AgentID} [agentId]
   * @param {string} [name]
   * @return {string}
   * @protected
   */
  _measurablesInPublicAndJoinedChannels(agentId, name = '') {
    assert(!!agentId, 'Agent ID is required.');
    return `(
      /* Measurables in Public and Joined Channels (${name}) */
      WITH channelIds AS (${this._publicAndJoinedChannels(agentId, name)})
      SELECT "Measurables"."id" FROM "Measurables"
      WHERE "Measurables"."channelId" IN (SELECT id FROM channelIds)
    )`;
  }

  /**
   * @protected
   * @param {string} [name]
   * @return {Sequelize.literal}
   */
  _measurablesInPublicChannelsLiteral(name = '') {
    return this.literal(
      this._measurablesInPublicChannels(name),
    );
  }

  /**
   * @todo: To use SQL replacements or bindings.
   * @param {string} [name]
   * @return {string}
   * @protected
   */
  _measurablesInPublicChannels(name = '') {
    return `(
      /* Measurables in Public Channels (${name}) */
      WITH channelIds AS (${this._publicChannels(name)})
      SELECT "Measurables"."id" FROM "Measurables"
      WHERE "Measurables"."channelId" IN (SELECT id FROM channelIds)
    )`;
  }

  /**
   * @protected
   * @param {string[]} statesIn
   * @param {Defs.ChannelID} channelIdIn
   * @param {string} [name]
   * @return {Sequelize.literal}
   */
  _withinMeasurablesLiteral(statesIn, channelIdIn, name = '') {
    return this.literal(
      this._withinMeasurables(statesIn, channelIdIn, name),
    );
  }

  /**
   * @todo: To use SQL replacements or bindings.
   * @param {string[] | null} statesIn
   * @param {Defs.ChannelID | null} channelIdIn
   * @param {string} [name]
   * @return {string}
   * @protected
   */
  _withinMeasurables(statesIn, channelIdIn, name = '') {
    const cond = [];
    const states = _.isArray(statesIn)
      ? statesIn.map((state) => `'${state}'`)
        .join(', ') : [];

    if (states.length > 0) cond.push(`("state" IN (${states}))`);
    if (!!channelIdIn) cond.push(`("channelId" = '${channelIdIn}')`);

    const where = cond.length > 0 ? `WHERE (${cond.join(' AND ')})` : '';

    return `(
      /* Within Measurables (${name}) */
      SELECT "id" FROM "Measurables"
      ${where}
    )`;
  }

  /**
   * @protected
   * @param {*[]} [list]
   * @return {*[]}
   */
  _getBooleansOfList(list = []) {
    return list.map((item) => {
      if (item === 'TRUE') {
        return true;
      }
      if (item === 'FALSE') {
        return false;
      }
      return item;
    });
  }

  /**
   * @param {Layers.Pagination} pagination
   * @returns {[string, string][]}
   * @protected
   */
  _getOrderFromPagination(pagination) {
    return pagination.getOrder()
      .map((item) => ([item.field, item.direction]));
  }

  /**
   * @return {string[][] | null}
   * @protected
   */
  _getDefaultOrder() {
    return [['createdAt', 'DESC']];
  }

  /**
   * @todo: - To do what exactly?
   * @param {Layers.Query} query
   * @return {string[][] | null}
   * @protected
   */
  _getOrderForOne(query) {
    if (!query) {
      return [['createdAt', 'DESC']];
    }
    if (query.sort === 0) {
      return null;
    }
    const sort = query.sort === 1 ? 'ASC' : 'DESC';
    return [['createdAt', sort]];
  }

  /**
   * @param {Layers.Models.ModelOptions} _options
   * @return {*}
   * @protected
   */
  _getAttributes(_options = {}) {
    return null;
  }

  /**
   * @param {Layers.Models.ModelOptions} _options
   * @return {*}
   * @protected
   */
  _getGroups(_options = {}) {
    return null;
  }

  /**
   * @protected
   * @param {string} name
   * @param {Layers.Models.ModelOptions} options
   * @returns {Promise<*>}
   */
  async _lockTable(name, options = {}) {
    this._assertInput({ options });
    const cond = {};
    this._extendGenericConditions(cond, options);
    await this.sequelize.query('SET LOCAL lock_timeout = \'3s\'', cond);
    return this.sequelize.query(`LOCK TABLE "${name}"`, cond);
  }

  /**
   * @protected
   * @param {string} name
   * @param {Layers.Models.ModelOptions} options
   * @returns {Promise<*>}
   */
  async _updateMaterializedView(name, options = {}) {
    this._assertInput({ options });
    const cond = {};
    this._extendGenericConditions(cond, options);
    return this.sequelize.query(`REFRESH MATERIALIZED VIEW "${name}"`, cond);
  }

  /**
   * @protected
   * @param {object} cond
   * @param {Layers.Models.ModelOptions} options
   */
  _extendGenericConditions(cond = {}, options = {}) {
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

  /**
   * @protected
   * @param {object} cond
   * @param {Layers.Models.ModelOptions} options
   */
  _extendAdvancedConditions(cond = {}, options = {}) {
    if (_.has(options, 'raw')) {
      cond.raw = options.raw;
    }
    if (_.has(options, 'attributes')) {
      cond.attributes = this._getAttributes(options);
    }
    if (_.has(options, 'group')) {
      cond.group = this._getGroups(options);
    }
    return cond;
  }

  /**
   * @todo: It is a temporary solution. Just to clean the application.
   * @param {object} input
   * @protected
   */
  _assertInput(input = {}) {
    for (const key in ModelPostgres.asserts) {
      const klass = ModelPostgres.asserts[key];
      const object = input[key];
      if (object === undefined) continue;
      if (!(object instanceof klass)) {
        this.log.warn(new Error(`"${key}" is not ${key} class.`));
      }
    }
  }
}

ModelPostgres.asserts = {
  data: Data,
  filter: Filter,
  options: Options,
  pagination: Pagination,
  params: Params,
  query: Query,
  restrictions: Restrictions,
};

module.exports = {
  ModelPostgres,
};
