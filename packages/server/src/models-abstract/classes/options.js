const assert = require('assert');
const _ = require('lodash');

class Options {
  /**
   * @todo: add type tests (assertion)
   * @param {Layers.AbstractModelsLayer.options} [options]
   */
  constructor(options) {
    const list = [
      'transaction', // object
      'lock', // bool
      'skipLocked', // bool
    ];

    _.each(list, (name) => {
      if (_.has(options, name)) {
        this[name] = _.get(options, name);
      }
    });
  }
}

module.exports = {
  Options,
};
