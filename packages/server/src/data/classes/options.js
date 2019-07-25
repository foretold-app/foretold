const _ = require('lodash');

/**
 * See "filter.js" comments.
 */
class Options {
  /**
   * @param {Layers.DataSourceLayer.options} [options]
   */
  constructor(options = {}) {
    const list = [
      'transaction', 'lock', ' skipLocked',
      'isAdmin', 'agentId', 'measuredByAgentId',
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
