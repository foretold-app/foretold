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
  }

  get data() {
    return this._data;
  }

  get total() {
    return this._total;
  }

  get edges() {
    return this._data.map((node) => ({ node, cursor: node.index }));
  }

  get pageInfo() {
    const { edges } = this;
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
