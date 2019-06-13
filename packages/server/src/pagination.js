const _ = require('lodash');

class Pagination {
  constructor(options = {}) {
    this.last = _.get(options, 'last');
    this.first = _.get(options, 'first');
    this.after = _.get(options, 'after');
    this.before = _.get(options, 'before');
  }
}

module.exports = {
  Pagination,
};
