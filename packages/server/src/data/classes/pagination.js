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
   * @param {[string, string][]} [options.order]
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
      this.order = _.get(options, 'order');
    } else {
      this.order = [['createdAt', 'DESC']];
    }
  }

  inspect() {
    utils.inspect(this);
  }
}

module.exports = {
  Pagination,
};
