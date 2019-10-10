const _ = require('lodash');

const { Model } = require('../models-abstract');
const { Options } = require('../models-abstract/classes/options');
const { Restrictions } = require('../models-abstract/classes/restrictions');

const structures = require('./classes/structures');

/**
 * @abstract
 */
class DataBase {
  /**
   * @todo: remove this property then
   */
  constructor() {
    this.model = new Model();
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
    const option$ = this._getModelOptions(options);
    const restrictions = this._getModelRestrictions(options);
    return this.model.createOne(data, restrictions, option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.params} [params]
   * @param {Layers.DataSourceLayer.query} [query]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<*>}
   */
  async getOne(params = {}, query = {}, options = {}) {
    const option$ = this._getModelOptions(options);
    const restrictions = this._getModelRestrictions(options);
    return this.model.getOne(params, query, restrictions, option$);
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
    const option$ = this._getModelOptions(options);
    const restrictions = this._getModelRestrictions(options);
    return this.model.getCount(params, query, restrictions, option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.params} [params]
   * @param {Layers.DataSourceLayer.data} [data]
   * @param {Layers.DataSourceLayer.options} options
   * @return {Promise<*>}
   */
  async updateOne(params = {}, data = {}, options = {}) {
    const option$ = this._getModelOptions(options);
    const restrictions = this._getModelRestrictions(options);
    return this.model.updateOne(params, data, restrictions, option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.params} [params]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<*>}
   */
  async deleteOne(params = {}, options = {}) {
    const option$ = this._getModelOptions(options);
    const restrictions = this._getModelRestrictions(options);
    return this.model.deleteOne(params, restrictions, option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.filter} [filter]
   * @param {Models.ObjectID} filter.userId
   * @param {Layers.DataSourceLayer.pagination} [pagination]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<{data: Models.Model[], total: number}>}
   */
  async getConnection(filter = {}, pagination = {}, options = {}) {
    const option$ = this._getModelOptions(options);
    const restrictions = this._getModelRestrictions(options);
    return this.model.getAllWithConnections(filter, pagination, restrictions, option$);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.filter} [filter]
   * @param {Layers.DataSourceLayer.pagination} [pagination]
   * @param {Layers.DataSourceLayer.options} [options]
   */
  async getAll(filter = {}, pagination = {}, options = {}) {
    const option$ = this._getModelOptions(options);
    const restrictions = this._getModelRestrictions(options);
    return this.model.getAll(filter, pagination, restrictions, option$);
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
    const option$ = this._getModelOptions(options);
    const restrictions = this._getModelRestrictions(options);
    return this.model.upsertOne(params, query, data, restrictions, option$);
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
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictions(options = {}) {
    return {};
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictionsForIncludedIntoChannel(options = {}) {
    const currentAgentId = _.get(options, 'currentAgentId');

    const withinPublicChannels = currentAgentId
      ? null
      : structures.withinPublicChannelsByChannelId();

    const withinPublicAndJoinedChannels = currentAgentId
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
    const currentAgentId = _.get(options, 'currentAgentId');
    return {
      measurableId: true,
      agentId: currentAgentId,
    };
  }
}

module.exports = {
  DataBase,
};
