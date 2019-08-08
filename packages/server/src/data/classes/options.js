const _ = require('lodash');

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
      'channelMemberId', // string
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
