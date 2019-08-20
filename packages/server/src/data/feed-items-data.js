const { DataBase } = require('./data-base');
const { FeedItemModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {FeedItemModel} FeedItemModel
 */
class FeedItemsData extends DataBase {

  constructor() {
    super();
    this.FeedItemModel = new FeedItemModel();
    this.model = this.FeedItemModel;
    this.defaultRestrictions = {
      channelId: true,
    };
  }

}

module.exports = {
  FeedItemsData,
};
