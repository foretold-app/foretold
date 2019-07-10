const _ = require('lodash');

class Filter {
  /**
   * @param {Layers.DataSourceLayer.params} [params]
   */
  constructor(params = {}) {
    this.agentId = _.get(params, 'agentId');
  }
}

module.exports = {
  Filter,
};
