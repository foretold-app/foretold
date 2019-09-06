const _ = require('lodash');
const utils = require('../../lib/utils');

const { splitBy } = require('../../lib/functions');

class ResponseAll {
  /**
   * @public
   * @param {*[]} data
   * @param {Function} totalFn
   * @param {number} offset
   * @param {number} spacedLimit
   */
  constructor(data, totalFn, offset, spacedLimit) {
    this._data = data;
    this._totalResult = null;
    this._totalFn = totalFn;
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

  getTotal() {
    if (!this._totalResult) {
      this._totalResult = this._totalFn();
    }
    return this._totalResult;
  }

  get total() {
    return this.getTotal();
  }

  get edges() {
    return this._edges;
  }

  get pageInfo() {
    return async function () {

      const start = _.head(this._edges);
      const end = _.last(this._edges);

      const startCursor = async () => _.get(start, 'cursor');
      const endCursor = async () => _.get(end, 'cursor');

      const hasNextPage = async () => {
        const total = await this.getTotal();
        return _.toNumber(endCursor) < (total - 1);
      };

      const hasPreviousPage = async () => {
        return _.toNumber(startCursor) > 0;
      };

      return {
        endCursor,
        startCursor,
        hasNextPage,
        hasPreviousPage,
      };
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
