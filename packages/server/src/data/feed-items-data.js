const { DataBase } = require('./data-base');
const { FeedItemModel } = require('../models');

/**
 * @implements {Layers.DataSource.Generic}
 * @property {FeedItemModel} model
 */
class FeedItemsData extends DataBase {
  constructor() {
    super();
    this.model = new FeedItemModel();
  }

  /**
   * @protected
   * @param {Layers.DataSource.Options} [options]
   * @return {Layers.Models.Restrictions}
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
