const models = require('../models');

const { Model } = require('../models-abstract');

class DataBase {

  /**
   * @todo: remove this property then
   */
  constructor() {
    this.models = models;
    this.model = new Model();
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.data} data
   * @param {Layers.DataSourceLayer.options} options
   * @return {Promise<*>}
   */
  async createOne(data, options) {
    return await this.model.createOne(data, options);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.params} [params]
   * @param {Layers.DataSourceLayer.query} [query]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<void>}
   */
  async getOne(params = {}, query = {}, options = {}) {
    return this.model.getOne(params, query, options);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.params} params
   * @param {Layers.DataSourceLayer.data} data
   * @param {Layers.DataSourceLayer.options} options
   * @return {Promise<*>}
   */
  async updateOne(params, data, options) {
    return await this.model.updateOne(params, data, options);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.params} params
   * @param {Layers.DataSourceLayer.options} options
   * @return {Promise<*>}
   */
  async deleteOne(params, options) {
    return await this.model.deleteOne(params, options);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.filter} [filter]
   * @param {Layers.DataSourceLayer.pagination} [pagination]
   * @param {Layers.DataSourceLayer.options} [options]
   */
  async getAll(filter = {}, pagination = {}, options = {}) {
    return this.model.getAll(filter, pagination, options);
  }

}

module.exports = {
  DataBase,
};


