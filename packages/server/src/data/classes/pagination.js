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
   */
  constructor(options = {}) {
    this.last = _.get(options, 'last');
    this.first = _.get(options, 'first');
    this.after = _.get(options, 'after');
    this.before = _.get(options, 'before');
    this.limit = _.get(options, 'limit');
    this.offset = _.get(options, 'offset');
  }

  /**
   * @public
   * @return {{offset: number | null, limit: number | null}}
   */
  getPagination() {
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
