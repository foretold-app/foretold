const _ = require('lodash');
const utils = require('../../lib/utils');

const { splitBy } = require('../../lib/functions');

class ResponseAll {
  /**
   * @public
   * @param {*[]} data
   * @param {number} total
   * @param {number} offset
   * @param {number} spacedLimit
   */
  constructor(data, total, offset, spacedLimit) {
    this._data = data;
    this._total = total;
    this._offset = offset;
    this._spacedLimit = spacedLimit;

    if (this._spacedLimit) {
      this._data = splitBy(this._data, this._spacedLimit);
    }

    this._data.map((item, index) => {
      item.index = this._offset + index;
      return item;
    });

    this._edges = this._data.map(node => ({ node, cursor: node.index }));
  }

  get data() {
    return this._data;
  }

  get total() {
    return this._total;
  }

  get edges() {
    return this._edges;
  }

  get pageInfo() {
    const start = _.head(this._edges);
    const end = _.last(this._edges);

    const startCursor = _.get(start, 'cursor');
    const endCursor = _.get(end, 'cursor');

    const hasNextPage = _.toNumber(endCursor) < (this._total - 1);
    const hasPreviousPage = _.toNumber(startCursor) > 0;

    return {
      hasNextPage,
      hasPreviousPage,
      startCursor,
      endCursor,
    };
  }

  /**
   * @public
   * @returns {* | null}
   */
  getFirst() {
    return _.head(this._data) || null;
  }

  inspect() {
    utils.inspect(this);
  }
}

module.exports = {
  ResponseAll,
};
