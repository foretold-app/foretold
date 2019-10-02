const assert = require('assert');
const _ = require('lodash');

const { DataBase } = require('./data-base');
const { GlobalSettingModel } = require('../models-abstract');
const { KenFacade } = require('../lib/ken-facade');

const { Params } = require('./classes/params');
const { Data } = require('./classes/data');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {FeedItemModel} FeedItemModel
 */
class GlobalSettingsData extends DataBase {
  constructor() {
    super();
    this.model = new GlobalSettingModel();
    this.kenFacade = this._getKenFacadeCached();
  }

  /**
   * @public
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
   * @public
   * @return {Promise<string | null>}
   */
  async getBotAgentId() {
    const main = await this.getMain();
    return _.get(main, 'botAgentId', null);
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
   * @todo: Never do like this.
   * @todo: These are shadowed promises.
   * @returns {Promise<KenFacade>}
   */
  async _getKenFacadeCached() {
    const { entityGraph } = await this.getMain();
    return new KenFacade(entityGraph);
  }

  /**
   * @todo: Never do like this.
   * @todo: These are shadowed promises.
   * @returns {Promise<KenFacade>}
   */
  async getKenFacadeCached() {
    return this.kenFacade;
  }
}

GlobalSettingsData.MAIN = 'main';

module.exports = {
  GlobalSettingsData,
};
