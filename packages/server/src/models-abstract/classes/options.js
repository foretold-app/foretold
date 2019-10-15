const _ = require('lodash');
const utils = require('../../lib/utils');

class Options {
  /**
   * @param {Layers.AbstractModelsLayer.options} [options]
   */
  constructor(options = {}) {
    const list = {
      transaction: (v) => _.isObject(v) || utils.none(v),
      lock: (v) => _.isBoolean(v) || utils.none(v),
      skipLocked: (v) => _.isBoolean(v) || utils.none(v),
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
