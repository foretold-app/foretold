const _ = require('lodash');
const utils = require('../../lib/utils');

/**
 * See "filter.js" comments.
 * @implements {Layers.DataSource.Query}
 */
class Query {
  /**
   * @todo: sort or Pagination.order?
   * @param {object} [query]
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
  col: (v) => _.isString(v) || utils.none(v),
  distinct: (v) => _.isBoolean(v) || utils.none(v),
  sort: (v) => _.isNumber(v) || utils.none(v),
};

Query.LIST = list;
Query.KEYS = Object.keys(list);

module.exports = {
  Query,
};
