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
    return {
      ...super._getDefaultRestrictionsForIncludedIntoChannel(options),
    };
  }

}

module.exports = {
  FeedItemsData,
};
