const _ = require('lodash');

/**
 * See "filter.js" comments.
 */
class Params {
  /**
   * @param {Layers.DataSourceLayer.params} [params]
   */
  constructor(params = {}) {
    const list = ['id', 'agentId'];

    _.each(list, (name) => {
      if (_.has(params, name)) {
        this[name] = _.get(params, name);
      }
    });
  }
}

module.exports = {
  Params,
};
