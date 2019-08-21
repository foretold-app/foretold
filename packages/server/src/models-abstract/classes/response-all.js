const _ = require('lodash');
const utils = require('../../lib/utils');

class ResponseAll {
  /**
   * @public
   * @param {*[]} data
   * @param {number} total
   */
  constructor(data, total) {
    this.data = data;
    this._total = total;
  }

  /**
   * @public
   * @return {*[]}
   */
  getData() {
    return this.data;
  }

  get total() {
    return this._total;
  }

  get edges() {
    return this.data.map(node => ({ node, cursor: node.index }));
  }

  get pageInfo() {
    const edges = this.edges;
    const start = _.head(edges);
    const end = _.last(edges);

    const startCursor = _.get(start, 'cursor');
    const endCursor = _.get(end, 'cursor');

    const hasNextPage = _.toNumber(endCursor) < (this.total - 1);
    const hasPreviousPage = _.toNumber(startCursor) > 0;

    return {
      hasNextPage,
      hasPreviousPage,
      startCursor,
      endCursor,
    };
  }

  inspect() {
    utils.inspect(this);
  }
}

module.exports = {
  ResponseAll,
};
