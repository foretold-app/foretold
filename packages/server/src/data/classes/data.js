const utils = require('../../lib/utils');

/**
 * See "filter.js" comments.
 * @implements {Layers.DataSourceLayer.Data}
 */
class Data {
  /**
   * @param {object} [data]
   */
  constructor(data) {
    utils.copy(this.constructor.name, data, this);
  }

  inspect() {
    utils.inspect(this);
  }
}

module.exports = {
  Data,
};
