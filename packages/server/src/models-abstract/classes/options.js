const utils = require('../../lib/utils');

class Options {
  /**
   * @todo: add type tests (assertion)
   * @param {Layers.AbstractModelsLayer.options} [options]
   */
  constructor(options = {}) {
    const list = [
      'transaction', // object
      'lock', // bool
      'skipLocked', // bool
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
