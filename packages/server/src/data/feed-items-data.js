const _ = require('lodash');

const { DataBase } = require('./data-base');
const { FeedItemModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {FeedItemModel} FeedItemModel
 */
class FeedItemsData extends DataBase {
  constructor() {
    super();
    this.model = new FeedItemModel();
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictions(options = {}) {
    return {
      ...super._getDefaultRestrictionsForIncludedIntoChannel(options),
    };
  }
}

module.exports = {
  FeedItemsData,
};
