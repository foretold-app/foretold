const _ = require('lodash');

const { DataBase } = require('./data-base');
const { FeedItemModel } = require('../models-abstract');

const structures = require('../structures');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {FeedItemModel} FeedItemModel
 */
class FeedItemsData extends DataBase {

  constructor() {
    super();
    this.FeedItemModel = new FeedItemModel();
    this.model = this.FeedItemModel;
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictions(options = {}) {
    const currentAgentId = _.get(options, 'currentAgentId');

    const withinPublicAndJoinedChannels = currentAgentId
      ? structures.withinPublicAndJoinedChannelsByChannelId(currentAgentId)
      : null;

    return { withinPublicAndJoinedChannels };
  }

}

module.exports = {
  FeedItemsData,
};
