const _ = require('lodash');
const utils = require('../../lib/utils');

/**
 * See "filter.js" comments.
 */
class Query {
  /**
   * @todo: sort or Pagination.order?
   * @param {Layers.DataSourceLayer.query} [query]
   */
  constructor(query = {}) {
    utils.copy(this.constructor.name, query, this);
    utils.test(this.constructor.name, Query.LIST, this);
    utils.diff(this.constructor.name, query, Query.LIST);
  }

  inspect() {
    utils.inspect(this);
  }
}

const list = {
  sort: (v) => _.isNumber(v) || utils.none(v),
};

Query.LIST = list;
Query.KEYS = Object.keys(list);

module.exports = {
  Query,
};
