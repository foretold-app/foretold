const _ = require('lodash');
const utils = require('../../lib/utils');

/**
 * See "filter.js" comments.
 */
class Query {
  /**
   * @param {Layers.DataSourceLayer.query} [query]
   */
  constructor(query = {}) {
    const list = {
      sort: v => _.isNumber(v) || utils.none(v),
    };
    utils.copy(this.constructor.name, query, this);
    utils.test(this.constructor.name, list, this);
    utils.diff(this.constructor.name, query, list);
  }

  inspect() {
    utils.inspect(this);
  }
}

module.exports = {
  Query,
};
