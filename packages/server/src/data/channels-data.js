const { DataBase } = require('./data-base');
const { AgentsData } = require('./agents-data');

const { ChannelModel } = require('../models');

const { Params } = require('./classes');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {ChannelModel} model
 */
class ChannelsData extends DataBase {
  constructor() {
    super();
    this.model = new ChannelModel();
    this.agents = new AgentsData();
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [_options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictions(_options = {}) {
    return { channelIdAsId: true };
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.data} data
   * @param {string} data.name
   * @return {Promise<Models.Channel>}
   */
  async createOne(data) {
    const channel = await this.getOne(new Params({ name: data.name }));
    if (channel) {
      return Promise.reject(new Error('Channel exists.'));
    }
    return super.createOne(data);
  }
}

module.exports = {
  ChannelsData,
};
