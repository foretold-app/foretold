const _ = require('lodash');

class Options {
  /**
   * @param {Layers.DataSourceLayer.options} [options]
   */
  constructor(options = {}) {
    this.type = _.get(options, 'transaction');
    this.isAdmin = _.get(options, 'isAdmin');
    this.agentId = _.get(options, 'agentId');
    this.measuredByAgentId = _.get(options, 'measuredByAgentId');
  }
}

module.exports = {
  Options,
};
