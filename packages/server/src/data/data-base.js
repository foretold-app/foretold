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
   * @param {Layers.DataSourceLayer.data} [data]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<*>}
   */
  async createOne(data, options = new DataOptions()) {
    const { option$, restriction$ } = this._getOptionsRestrictions(options);
    return this.model.createOne(data, restriction$, option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.params} [params]
   * @param {Layers.DataSourceLayer.query} [query]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<*>}
   */
  async getOne(params, query, options = new DataOptions()) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.getOne(params, query, restriction$, option$);
  }

  /**
   * @todo: later fix "col" argument
   * @public
   * @param {Layers.DataSourceLayer.params} [params]
   * @param {Layers.DataSourceLayer.query} [query]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<*>}
   */
  async getCount(params, query, options = new DataOptions()) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.getCount(params, query, restriction$, option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.params} [params]
   * @param {Layers.DataSourceLayer.data} [data]
   * @param {Layers.DataSourceLayer.options} options
   * @return {Promise<*>}
   */
  async updateOne(params, data, options = new DataOptions()) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.updateOne(params, data, restriction$, option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.params} [params]
   * @param {Layers.DataSourceLayer.query} [query]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<*>}
   */
  async deleteOne(params, query, options = new DataOptions()) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.deleteOne(params, query, restriction$, option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.params} [params]
   * @param {Layers.DataSourceLayer.query} [query]
   * @param {Layers.DataSourceLayer.data} [data]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<*>}
   */
  async upsertOne(params, query, data, options = new DataOptions()) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.upsertOne(params, query, data, restriction$, option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.filter} [filter]
   * @param {Models.AgentID} filter.userId
   * @param {Layers.DataSourceLayer.pagination} [pagination]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<{data: Models.Model[], total: number}>}
   */
  async getConnection(filter, pagination, options = new DataOptions()) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.getAllWithConnections(
      filter, pagination, restriction$, option$,
    );
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.filter} [filter]
   * @param {Layers.DataSourceLayer.pagination} [pagination]
   * @param {Layers.DataSourceLayer.options} [options]
   */
  async getAll(filter, pagination, options = new DataOptions()) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.getAll(filter, pagination, restriction$, option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<*>}
   */
  async lock(options = new DataOptions()) {
    const { option$ } = this._getOptionsRestrictions(options);
    return this.model.lock(option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<*>}
   */
  async updateMaterializedView(options = new DataOptions()) {
    const { option$ } = this._getOptionsRestrictions(options);
    return this.model.updateMaterializedView(option$);
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {{
   *  option$: Layers.AbstractModelsLayer.options,
   *  restriction$: Layers.AbstractModelsLayer.restrictions,
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
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.options}
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
   * @param {Layers.DataSourceLayer.options} [_options]
   * @return {Layers.AbstractModelsLayer.options}
   */
  _getDefaultOptions(_options = new DataOptions()) {
    return {};
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
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
   * @param {Layers.DataSourceLayer.options} [_options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictions(_options = new DataOptions()) {
    return {};
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
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
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
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
   * @param {Layers.DataSourceLayer.options} [options]
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
