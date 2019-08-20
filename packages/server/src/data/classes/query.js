const utils = require('../../lib/utils');

/**
 * See "filter.js" comments.
 */
class Query {
  /**
   * @param {Layers.DataSourceLayer.query} [query]
   */
  constructor(query = {}) {
    utils.copy(this.constructor.name, query, this);
  }
}

module.exports = {
  Query,
};
