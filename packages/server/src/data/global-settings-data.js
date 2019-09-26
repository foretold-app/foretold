const assert = require('assert');
const _ = require('lodash');

const { DataBase } = require('./data-base');
const { GlobalSettingModel } = require('../models-abstract');

const { Params } = require('./classes/params');
const { Data } = require('./classes/data');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {FeedItemModel} FeedItemModel
 */
class GlobalSettingsData extends DataBase {
  constructor() {
    super();
    this.GlobalSettingModel = new GlobalSettingModel();
    this.model = this.GlobalSettingModel;
  }

  /**
   * @param {object} incomingData
   * @return {Promise<Models.Model>}
   */
  async updateEntityGraph(incomingData) {
    assert(_.isObject(incomingData), 'EntityGraph should be an object');
    const params = new Params({ name: GlobalSettingsData.MAIN });
    const data = new Data({ entityGraph: incomingData });
    return this.updateOne(params, data);
  }

  /**
   * @return {Promise<string | null>}
   */
  async getBotAgentId() {
    const main = await this.getMain();
    return _.get(main, 'botAgentId', null);
  }

  /**
   * @return {Promise<string>}
   */
  async getMain() {
    const params = new Params({ name: GlobalSettingsData.MAIN });
    return this.getOne(params);
  }
}

GlobalSettingsData.MAIN = 'main';

module.exports = {
  GlobalSettingsData,
};
