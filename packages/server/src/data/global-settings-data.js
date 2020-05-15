const assert = require('assert');
const _ = require('lodash');

const { DataBase } = require('./data-base');
const { GlobalSettingModel } = require('./models');
const { KenFacade } = require('../lib/ken-facade');

const { Params } = require('./classes');
const { Data } = require('./classes');

/**
 * @implements {Layers.DataSource.DataGeneric}
 * @property {FeedItemModel} model
 */
class GlobalSettingsData extends DataBase {
  constructor() {
    super();
    this.model = new GlobalSettingModel();
  }

  /**
   * @public
   * @param {object} incomingData
   * @return {Promise<Models.Definition>}
   */
  async updateEntityGraph(incomingData) {
    assert(_.isObject(incomingData), 'EntityGraph should be an object.');
    const params = new Params({ name: GlobalSettingsData.MAIN });
    const data = new Data({ entityGraph: incomingData });
    return this.updateOne(params, data);
  }

  /**
   * @public
   * @param {object} main
   * @return {Promise<string | null>}
   */
  async getBotAgentId(main) {
    const mainSet = main || await this.getMain();
    return _.get(mainSet, 'botAgentId', null);
  }

  /**
   * @public
   * @return {Promise<object>}
   */
  async getMain() {
    const params = new Params({ name: GlobalSettingsData.MAIN });
    return this.getOne(params);
  }

  /**
   * @public
   * @returns {Promise<KenFacade>}
   */
  async getKen() {
    const { entityGraph } = await this.getMain();
    return new KenFacade(entityGraph);
  }
}

GlobalSettingsData.MAIN = 'main';

module.exports = {
  GlobalSettingsData,
};
