const _ = require('lodash');

const { DataBase } = require('./data-base');
const { AgentsData } = require('./agents-data');

const { ChannelModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
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
    const channel = await this.getOne({ name: data.name });
    if (channel) {
      return Promise.reject(new Error('Channel exists.'));
    }
    return super.createOne(data);
  }

  /**
   * @public
   * @param {Models.ObjectID} id
   * @return {Promise<Model[]>}
   */
  async getAgentsByChannelId(id) {
    return this.model.getAgentsByChannelId(id);
  }

  /**
   * @public
   * @param {Models.ObjectID} id
   * @return {Promise<Model>}
   */
  async getCreatorByChannelId(id) {
    const channel = await this.getOne({ id });
    const creatorId = _.get(channel, 'creatorId');
    if (!creatorId) return null;
    return this.agents.getOne({ id: creatorId });
  }
}

module.exports = {
  ChannelsData,
};
