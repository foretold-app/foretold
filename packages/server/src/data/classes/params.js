const _ = require('lodash');
const utils = require('../../lib/utils');

/**
 * See "filter.js" comments.
 */
class Params {
  /**
   * @param {Layers.DataSourceLayer.params} [params]
   */
  constructor(params = {}) {
    const list = {
      id: (v) => _.isString(v) || utils.none(v),
      agentId: (v) => _.isString(v) || utils.none(v),
      name: (v) => _.isString(v) || utils.none(v),
      auth0Id: (v) => _.isString(v) || utils.none(v),
      measurableId: (v) => _.isString(v) || utils.none(v),
      competitorType: (v) => _.isString(v) || _.isArray(v) || utils.none(v),
      seriesId: (v) => _.isString(v) || utils.none(v),
      email: (v) => _.isString(v) || utils.none(v),
      channelId: (v) => _.isString(v) || utils.none(v),
      isEmailVerified: (v) => _.isBoolean(v) || utils.none(v),
    };
    utils.extend(this.constructor.name, params, list, this);
    utils.test(this.constructor.name, list, this);
    utils.diff(this.constructor.name, params, list);
  }

  inspect() {
    utils.inspect(this);
  }
}

module.exports = {
  Params,
};
