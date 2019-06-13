const _ = require('lodash');

class Pagination {
  constructor(options = {}) {
    this.last = _.get(options, 'last', 0);
    this.first = _.get(options, 'first', 0);
    this.after = _.get(options, 'after', 0) * 1;
    this.before = _.get(options, 'before', 0) * 1;
  }
}

module.exports = {
  Pagination,
};
