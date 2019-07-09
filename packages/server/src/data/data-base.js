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
   * @param {Layers.DataSourceLayer.data} [data]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<*>}
   */
  async createOne(data = {}, options = {}) {
    const restrictions = { ...options };
    return this.model.createOne(data, restrictions);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.params} [params]
   * @param {Layers.DataSourceLayer.query} [query]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<void>}
   */
  async getOne(params = {}, query = {}, options = {}) {
    const restrictions = { ...options };
    return this.model.getOne(params, query, restrictions);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.params} [params]
   * @param {Layers.DataSourceLayer.data} [data]
   * @param {Layers.DataSourceLayer.options} options
   * @return {Promise<*>}
   */
  async updateOne(params = {}, data = {}, options = {}) {
    const restrictions = { ...options };
    return this.model.updateOne(params, data, restrictions);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.params} [params]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<*>}
   */
  async deleteOne(params = {}, options = {}) {
    const restrictions = { ...options };
    return this.model.deleteOne(params, restrictions);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.filter} [filter]
   * @param {Layers.DataSourceLayer.pagination} [pagination]
   * @param {Layers.DataSourceLayer.options} [options]
   */
  async getAll(filter = {}, pagination = {}, options = {}) {
    const restrictions = { ...options };
    return this.model.getAll(filter, pagination, restrictions);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.params} [params]
   * @param {Layers.DataSourceLayer.query} [query]
   * @param {Layers.DataSourceLayer.data} [data]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<*>}
   */
  async getCreateOne(params = {}, query = {}, data = {}, options = {}) {
    const restrictions = { ...options };
    return this.model.getCreateOne(params, query, data, restrictions);
  }

}

module.exports = {
  DataBase,
};


