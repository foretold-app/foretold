const _ = require('lodash');

class Options {
  /**
   * @param {Layers.DataSourceLayer.options} [options]
   */
  constructor(options = {}) {
    this.transaction = _.get(options, 'transaction');
    this.lock = _.get(options, 'lock');
    this.skipLocked = _.get(options, 'skipLocked');

    this.isAdmin = _.get(options, 'isAdmin');
    this.agentId = _.get(options, 'agentId');
    this.measuredByAgentId = _.get(options, 'measuredByAgentId');
  }
}

module.exports = {
  Options,
};
