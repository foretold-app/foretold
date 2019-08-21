const utils = require('../../lib/utils');

/**
 * See "filter.js" comments.
 */
class Options {

  /**
   * @todo: add type tests (assertion)
   * @param {Layers.DataSourceLayer.options} [options]
   */
  constructor(options = {}) {
    const list = [
      'transaction', // object
      'lock', // bool
      'skipLocked', // bool

      'isAdmin',  // bool
      'agentId',  // string
      'measuredByAgentId', // string
      'currentAgentId', // string
    ];
    utils.extend(this.constructor.name, options, list, this);
    utils.diff(this.constructor.name, options, list);
  }

  inspect() {
    utils.inspect(this);
  }
}

module.exports = {
  Options,
};
