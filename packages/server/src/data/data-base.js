const _ = require('lodash');

const { Model } = require('../models');
const { Options: ModelsOptions } = require('../models/classes');
const { Options: DataOptions } = require('./classes');
const { Restrictions } = require('../models/classes');
const logger = require('../lib/log');

const { structures } = require('./classes');

/**
 * @abstract
 */
class DataBase {
  /**
   * @todo: remove this property then
   */
  constructor() {
    this.model = new Model();
    this.log = logger.module('data/data-base');
  }

  /**
   * @public
   * @return {Promise<Layers.Transaction>}
   */
  async getTransaction() {
    return this.model.getTransaction();
  }

  /**
   * @public
   * @param {Layers.Transaction} transaction
   * @return {Promise<*>}
   */
  async commit(transaction) {
    return this.model.commit(transaction);
  }

  /**
   * @public
   * @param {Layers.Transaction} transaction
   * @return {Promise<*>}
   */
  async rollback(transaction) {
    return this.model.rollback(transaction);
  }

  /**
   * @public
   * @param {Layers.DataSource.Data} [data]
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Promise<*>}
   */
  async createOne(data, options = new DataOptions()) {
    const { option$, restriction$ } = this._getOptionsRestrictions(options);
    return this.model.createOne(data, restriction$, option$);
  }

  /**
   * @public
   * @param {Layers.DataSource.DataParams} [params]
   * @param {Layers.DataSource.DataQuery} [query]
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Promise<*>}
   */
  async getOne(params, query, options = new DataOptions()) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.getOne(params, query, restriction$, option$);
  }

  /**
   * @todo: later fix "col" argument
   * @public
   * @param {Layers.DataSource.DataParams} [params]
   * @param {Layers.DataSource.DataQuery} [query]
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Promise<*>}
   */
  async getCount(params, query, options = new DataOptions()) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.getCount(params, query, restriction$, option$);
  }

  /**
   * @public
   * @param {Layers.DataSource.DataParams} [params]
   * @param {Layers.DataSource.Data} [data]
   * @param {Layers.DataSource.DataOptions} options
   * @return {Promise<*>}
   */
  async updateOne(params, data, options = new DataOptions()) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.updateOne(params, data, restriction$, option$);
  }

  /**
   * @public
   * @param {Layers.DataSource.DataParams} [params]
   * @param {Layers.DataSource.DataQuery} [query]
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Promise<*>}
   */
  async deleteOne(params, query, options = new DataOptions()) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.deleteOne(params, query, restriction$, option$);
  }

  /**
   * @public
   * @param {Layers.DataSource.DataParams} [params]
   * @param {Layers.DataSource.DataQuery} [query]
   * @param {Layers.DataSource.Data} [data]
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Promise<*>}
   */
  async upsertOne(params, query, data, options = new DataOptions()) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.upsertOne(params, query, data, restriction$, option$);
  }

  /**
   * @public
   * @param {Layers.DataSource.DataFilter} [filter]
   * @param {Models.AgentID} filter.userId
   * @param {Layers.DataSource.DataPagination} [pagination]
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Promise<{data: Models.Definition[], total: number}>}
   */
  async getConnection(filter, pagination, options = new DataOptions()) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.getAllWithConnections(
      filter, pagination, restriction$, option$,
    );
  }

  /**
   * @public
   * @param {Layers.DataSource.DataFilter} [filter]
   * @param {Layers.DataSource.DataPagination} [pagination]
   * @param {Layers.DataSource.DataOptions} [options]
   */
  async getAll(filter, pagination, options = new DataOptions()) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.getAll(filter, pagination, restriction$, option$);
  }

  /**
   * @public
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Promise<*>}
   */
  async lock(options = new DataOptions()) {
    const { option$ } = this._getOptionsRestrictions(options);
    return this.model.lock(option$);
  }

  /**
   * @public
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Promise<*>}
   */
  async updateMaterializedView(options = new DataOptions()) {
    const { option$ } = this._getOptionsRestrictions(options);
    return this.model.updateMaterializedView(option$);
  }

  /**
   * @protected
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {{
   *  option$: Layers.Models.ModelOptions,
   *  restriction$: Layers.Models.ModelRestrictions,
   * }}
   */
  _getOptionsRestrictions(options = new DataOptions()) {
    const {
      separatedOptions,
      separatedRestrictions,
    } = this._separateDataOptions(options);
    const option$ = this._getModelOptions(separatedOptions);
    const restriction$ = this._getModelRestrictions(separatedRestrictions);
    return { option$, restriction$ };
  }

  /**
   * @protected
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Layers.Models.ModelOptions}
   */
  _getModelOptions(options = new DataOptions()) {
    if (_.isEmpty(options)) return new ModelsOptions({});

    return new ModelsOptions({
      ...options,
      ...this._getDefaultOptions(options),
    });
  }

  /**
   * @protected
   * @param {Layers.DataSource.DataOptions} [_options]
   * @return {Layers.Models.ModelOptions}
   */
  _getDefaultOptions(_options = new DataOptions()) {
    return {};
  }

  /**
   * @protected
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Layers.Models.ModelRestrictions}
   */
  _getModelRestrictions(options = new DataOptions()) {
    if (_.isEmpty(options)) return new Restrictions({});

    return new Restrictions({
      ...options,
      ...this._getDefaultRestrictions(options),
    });
  }

  /**
   * @protected
   * @param {Layers.DataSource.DataOptions} [_options]
   * @return {Layers.Models.ModelRestrictions}
   */
  _getDefaultRestrictions(_options = new DataOptions()) {
    return {};
  }

  /**
   * @protected
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Layers.Models.ModelRestrictions}
   */
  _getDefaultRestrictionsForIncludedIntoChannel(options = new DataOptions()) {
    const currentAgentId = _.get(options, 'currentAgentId', null);

    const withinPublicChannels = !!currentAgentId
      ? null
      : structures.withinPublicChannelsByChannelId();

    const withinPublicAndJoinedChannels = !!currentAgentId
      ? structures.withinPublicAndJoinedChannelsByChannelId(currentAgentId)
      : null;

    return {
      withinPublicChannels,
      withinPublicAndJoinedChannels,
    };
  }

  /**
   * @protected
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Layers.Models.ModelRestrictions}
   */
  _getDefaultRestrictionsForIncludedIntoMeasurables(
    options = new DataOptions(),
  ) {
    const currentAgentId = _.get(options, 'currentAgentId', null);

    return {
      measurableId: true,
      agentId: currentAgentId,
    };
  }

  /**
   * @protected
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {{separatedOptions: Object, separatedRestrictions: Object}}
   */
  _separateDataOptions(options = new DataOptions()) {
    const separatedOptions = _.omit(options, Restrictions.KEYS);
    const separatedRestrictions = _.omit(options, ModelsOptions.KEYS);
    return { separatedOptions, separatedRestrictions };
  }
}

module.exports = {
  DataBase,
};
