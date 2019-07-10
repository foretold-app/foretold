const _ = require('lodash');

class Params {
  /**
   * @param {Layers.DataSourceLayer.params} [params]
   */
  constructor(params = {}) {
    this.id = _.get(params, 'id');
    this.agentId = _.get(params, 'agentId');
  }
}

module.exports = {
  Params,
};
