const _ = require('lodash');

const utils = require('../../lib/utils');

/**
 * See "filter.js" comments.
 * @implements {Layers.Pagination}
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
   * @param {object} [context]
   * @param {Defs.AgentID} [context.agentId]
   */
  constructor(options = {}, context = {}) {
    this._context = {};

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
    if (_.has(context, 'agentId')) {
      this._context.agentId = _.get(context, 'agentId', null);
    }
  }

  /**
   * @param {object} options
   * @param {Layers.orderList} options.order
   * @returns {Layers.orderList}
   * @protected
   */
  _getOrder(options) {
    const orderInput = _.get(options, 'order', []);
    const orderArray = _.isArray(orderInput) ? orderInput : [];
    return orderArray
      .map((item) => {
        const field = _.get(item, 'field', null);
        const direction = _.get(item, 'direction', null);
        return { field, direction };
      })
      .filter((item) => _.isString(item.field))
      .filter((item) => _.isString(item.direction));
  }

  /**
   * @public
   * @returns {boolean}
   */
  isOrderSet() {
    return !!this.order && this.order.length > 0;
  }

  /**
   * @public
   * @returns {{field: string, direction: string}[]}
   */
  getOrder() {
    return this.order;
  }

  /**
   * @todo: Adds a limit to the "limit".
   * @public
   * @return {{offset: number | null, limit: number | null}}
   */
  getPagination() {
    if (this.first) {
      const limit = this.first;
      const offset = this.after ? this.after + 1 : null;
      return { limit, offset };
    }
    if (this.last) {
      const limit = this.last;
      const offset = this.before ? this.before - this.last : null;
      return { limit, offset };
    }
    return {
      limit: this.limit || 10,
      offset: null,
    };
  }

  /**
   * @public
   */
  inspect() {
    utils.inspect(this);
  }

  /**
   * @param {string} name
   * @returns {*}
   */
  getContext(name) {
    return _.get(this._context, name);
  }
}

module.exports = {
  Pagination,
};
