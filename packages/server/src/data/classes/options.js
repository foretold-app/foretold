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
    utils.extend(this.constructor.name, options, Options.LIST, this);
    utils.test(this.constructor.name, Options.LIST, this);
    utils.diff(this.constructor.name, options, Options.LIST);
  }

  inspect() {
    utils.inspect(this);
  }
}

const list = {
  // Copy from "models/options".
  attributes: (v) => _.isBoolean(v) || _.isObject(v) || utils.none(v),
  group: (v) => _.isBoolean(v) || utils.none(v),
  lock: (v) => _.isBoolean(v) || _.isPlainObject(v) || utils.none(v),
  raw: (v) => _.isBoolean(v) || utils.none(v),
  skipLocked: (v) => _.isBoolean(v) || utils.none(v),
  transaction: (v) => _.isObject(v) || utils.none(v),

  agentId: (v) => _.isString(v) || utils.none(v),
  currentAgentId: (v) => _.isString(v) || utils.none(v),
  isAdmin: (v) => _.isBoolean(v) || utils.none(v),
  measuredByAgentId: (v) => _.isString(v) || utils.none(v),
};

Options.LIST = list;
Options.KEYS = Object.keys(list);

module.exports = {
  Options,
};
