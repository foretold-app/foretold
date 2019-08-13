const _ = require('lodash');

/**
 * See "filter.js" comments.
 */
class Data {
  /**
   * @param {Layers.DataSourceLayer.data} [data]
   */
  constructor(data) {
    _.each(data, (value, key) => {
      this[key] = value;
    });
  }
}

module.exports = {
  Data,
};
