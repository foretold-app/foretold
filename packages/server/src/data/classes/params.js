const _ = require('lodash');
const utils = require('../../lib/utils');

/**
 * See "filter.js" comments.
 * @implements {Layers.DataSource.DataParams}
 */
class Params {
  /**
   * @param {object} [params]
   */
  constructor(params = {}) {
    utils.extend(this.constructor.name, params, Params.LIST, this);
    utils.test(this.constructor.name, Params.LIST, this);
    utils.diff(this.constructor.name, params, Params.LIST);
  }

  inspect() {
    utils.inspect(this);
  }
}

const list = {
  agentId: (v) => _.isString(v) || utils.none(v),
  auth0Id: (v) => _.isString(v) || utils.none(v),
  channelId: (v) => _.isString(v) || utils.none(v),
  competitorType: (v) => _.isString(v) || _.isArray(v) || utils.none(v),
  email: (v) => _.isString(v) || utils.none(v),
  id: (v) => _.isString(v) || utils.none(v),
  isEmailVerified: (v) => _.isArray(v) || utils.none(v),
  isNotEmailVerified: (v) => _.isBoolean(v) || utils.none(v),
  measurableId: (v) => _.isString(v) || utils.none(v),
  measurableIds: (v) => _.isArray(v) || utils.none(v),
  measurementId: (v) => _.isString(v) || utils.none(v),
  name: (v) => _.isString(v) || utils.none(v),
  notebookId: (v) => _.isString(v) || utils.none(v),
  seriesId: (v) => _.isString(v) || utils.none(v),
};

Params.LIST = list;
Params.KEYS = Object.keys(list);

module.exports = {
  Params,
};
