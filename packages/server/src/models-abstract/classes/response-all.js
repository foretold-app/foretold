const _ = require('lodash');
const utils = require('../../lib/utils');

class ResponseAll {
  /**
   * @public
   * @param {Function} dataFn
   * @param {Function} totalFn
   * @param {number} offset
   */
  constructor(dataFn, totalFn, offset) {
    this._dataFn = dataFn;
    this._totalFn = totalFn;
    this._offset = offset;
    this._dataResult = null;
    this._totalResult = null;
  }

  getData() {
    if (!this._dataResult) {
      this._dataResult = this._dataFn()
        .then(data => data.map((item, index) => {
          item.index = this._offset + index;
          return item;
        }));
    }
    return this._dataResult;
  }

  getTotal() {
    if (!this._totalResult) {
      this._totalResult = this._totalFn();
    }
    return this._totalResult;
  }

  getEdges() {
    return this.getData()
      .then(data => data.map(node => ({ node, cursor: node.index })));
  }

  get data() {
    return this.getData();
  }

  get total() {
    return this.getTotal();
  }

  get edges() {
    return this.getEdges();
  }

  get pageInfo() {
    return async function () {

      const startCursor = async () => {
        const start = _.head(await this.getEdges());
        return _.get(start, 'cursor');
      };

      const endCursor = async () => {
        const end = _.last(await this.getEdges());
        return _.get(end, 'cursor');
      };

      const hasNextPage = async () => {
        const total = await this.getTotal();
        return _.toNumber(await endCursor()) < (total - 1);
      };

      const hasPreviousPage = async () => {
        return _.toNumber(await startCursor()) > 0;
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
  async getFirst() {
    return _.head(await this.getData()) || null;
  }

  inspect() {
    utils.inspect(this);
  }
}

module.exports = {
  ResponseAll,
};
