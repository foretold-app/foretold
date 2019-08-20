const utils = require('../../lib/utils');

/**
 * See "filter.js" comments.
 */
class Data {
  /**
   * @param {Layers.DataSourceLayer.data} [data]
   */
  constructor(data) {
    utils.copy(this.constructor.name, data, this);
  }
}

module.exports = {
  Data,
};
