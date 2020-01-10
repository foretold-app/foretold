const _ = require('lodash');
const utils = require('../../lib/utils');

/**
 * See "filter.js" comments.
 */
class Options {
  /**
   * @param {Layers.DataSourceLayer.options} [options]
   */
  constructor(options = {}) {
    const list = {
      lock: (v) => _.isBoolean(v) || utils.none(v),
      skipLocked: (v) => _.isBoolean(v) || utils.none(v),
      transaction: (v) => _.isObject(v) || utils.none(v),

      agentId: (v) => _.isString(v) || utils.none(v),
      currentAgentId: (v) => _.isString(v) || utils.none(v),
      isAdmin: (v) => _.isBoolean(v) || utils.none(v),
      measuredByAgentId: (v) => _.isString(v) || utils.none(v),
    };

    utils.extend(this.constructor.name, options, list, this);
    utils.test(this.constructor.name, list, this);
    utils.diff(this.constructor.name, options, list);
  }

  inspect() {
    utils.inspect(this);
  }
}

module.exports = {
  Options,
};
