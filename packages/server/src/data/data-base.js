const _ = require('lodash');

const { Model } = require('../models-abstract');
const { Options } = require('../models-abstract/classes');
const { Restrictions } = require('../models-abstract/classes');
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
   * @return {Promise<*>}
   */
  async getTransaction() {
    return this.model.getTransaction();
  }

  /**
   * @public
   * @param {*} transaction
   * @return {Promise<*>}
   */
  async commit(transaction) {
    return this.model.commit(transaction);
  }

  /**
   * @public
   * @param {*} transaction
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
  async createOne(data = {}, options = {}) {
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
  async getOne(params = {}, query = {}, options = {}) {
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
  async getCount(params = {}, query = {}, options = {}) {
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
  async updateOne(params = {}, data = {}, options = {}) {
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
  async deleteOne(params = {}, query = {}, options = {}) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.deleteOne(params, query, restriction$, option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.filter} [filter]
   * @param {Models.AgentID} filter.userId
   * @param {Layers.DataSourceLayer.pagination} [pagination]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<{data: Models.Model[], total: number}>}
   */
  async getConnection(filter = {}, pagination = {}, options = {}) {
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
  async getAll(filter = {}, pagination = {}, options = {}) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.getAll(filter, pagination, restriction$, option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.params} [params]
   * @param {Layers.DataSourceLayer.query} [query]
   * @param {Layers.DataSourceLayer.data} [data]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<*>}
   */
  async upsertOne(params = {}, query = {}, data = {}, options = {}) {
    const { restriction$, option$ } = this._getOptionsRestrictions(options);
    return this.model.upsertOne(params, query, data, restriction$, option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<*>}
   */
  async lock(options = {}) {
    const { option$ } = this._getOptionsRestrictions(options);
    return this.model.lock(option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<*>}
   */
  async updateMaterializedView(options = {}) {
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
  _getOptionsRestrictions(options = {}) {
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
  _getModelOptions(options = {}) {
    if (_.isEmpty(options)) return new Options({});

    return new Options({
      ...options,
      ...this._getDefaultOptions(options),
    });
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [_options]
   * @return {Layers.AbstractModelsLayer.options}
   */
  _getDefaultOptions(_options = {}) {
    return {};
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getModelRestrictions(options = {}) {
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
  _getDefaultRestrictions(_options = {}) {
    return {};
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictionsForIncludedIntoChannel(options = {}) {
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
  _getDefaultRestrictionsForIncludedIntoMeasurables(options = {}) {
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
  _separateDataOptions(options = {}) {
    const separatedOptions = _.omit(options, Restrictions.KEYS);
    const separatedRestrictions = _.omit(options, Options.KEYS);
    return { separatedOptions, separatedRestrictions };
  }
}

module.exports = {
  DataBase,
};
