const _ = require('lodash');

const utils = require('../../lib/utils');

/**
 * See "filter.js" comments.
 */
class Pagination {
  /**
   * @public
   * @param {object} [options]
   * @param {number} [options.last]
   * @param {number} [options.first]
   * @param {string} [options.after]
   * @param {string} [options.before]
   * @param {number} [options.limit]
   * @param {number} [options.offset]
   * @param {Layers.orderList} [options.order]
   */
  constructor(options = {}) {
    if (_.has(options, 'last')) {
      this.last = _.get(options, 'last', 0);
    }
    if (_.has(options, 'first')) {
      this.first = _.get(options, 'first', 0);
    }

    if (_.has(options, 'after')) {
      this.after = _.get(options, 'after', 0) * 1;
    }
    if (_.has(options, 'before')) {
      this.before = _.get(options, 'before', 0) * 1;
    }

    if (_.has(options, 'limit')) {
      this.limit = _.get(options, 'limit');
    }
    if (_.has(options, 'offset')) {
      this.offset = _.get(options, 'offset');
    }

    if (_.has(options, 'order')) {
      this.order = this._getOrder(options);
    }
  }

  /**
   * @param {object} options
   * @param {Layers.orderList} options.order
   * @returns {Layers.orderList}
   * @private
   */
  _getOrder(options) {
    const orderInput = _.get(options, 'order');
    const orderArray = _.isArray(orderInput) ? orderInput : [];
    return orderArray
      .filter((item) => _.has(item, 'field'))
      .filter((item) => _.has(item, 'direction'))
      .map((item) => ({
        field: _.get(item, 'field'),
        direction: _.get(item, 'direction'),
      }));
  }

  /**
    * @returns {boolean}
   */
  isOrderSet() {
    return !!this.order && this.order.length > 0;
  }

  /**
   * @returns {{field: any, direction: any}[]}
   */
  getOrder() {
    return this.order;
  }

  /**
   * @public
   * @return {{offset: number | null, limit: number | null}}
   */
  getPagination2() {
    if (this.first) {
      const limit = this.first;
      const offset = this.after ? this.after * 1 + 1 : null;
      return { limit, offset };
    } else if (this.last) {
      const limit = this.last;
      const offset = this.before ? this.before * 1 - this.last : null;
      return { limit, offset };
    }
    return { limit: 10, offset: null };
  }

  inspect() {
    utils.inspect(this);
  }
}

module.exports = {
  Pagination,
};
