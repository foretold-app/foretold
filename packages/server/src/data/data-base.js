const _ = require('lodash');

const models = require('../models');
const { Model } = require('../models-abstract');

class DataBase {

  /**
   * @todo: remove this property then
   */
  constructor() {
    this.models = models;
    this.model = new Model();
    this.modelOptionsList = [
      'transaction',
    ];
    this.modelRestrictionsList = [
      'isAdmin', 'agentId', 'measuredByAgentId',
      'userId', 'channelId', 'measurableId',
      'measuredByAgentId',
    ];
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
   * @return {Promise<void>}
   */
  async getOne(params = {}, query = {}, options = {}) {
    const option$ = this._getModelOptions(options);
    const restrictions = this._getModelRestrictions(options);
    return this.model.getOne(params, query, restrictions, option$);
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
    return _.pick(options, this.modelOptionsList);
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.options}
   */
  _getModelRestrictions(options = {}) {
    return _.pick(options, this.modelRestrictionsList);
  }
}

module.exports = {
  DataBase,
};


