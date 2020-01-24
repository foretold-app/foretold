const _ = require('lodash');
const utils = require('../../lib/utils');

class Restrictions {
  /**
   * @param {Layers.AbstractModelsLayer.restrictions} [options]
   */
  constructor(options) {
    utils.extend(this.constructor.name, options, Restrictions.LIST, this);
    utils.test(this.constructor.name, Restrictions.LIST, this);
    utils.diff(this.constructor.name, options, Restrictions.LIST);
  }

  inspect() {
    utils.inspect(this);
  }
}

const list = {
  agentId: (v) => _.isString(v) || utils.none(v),
  channelId: (v) => _.isBoolean(v) || utils.none(v),
  channelIdAsId: (v) => _.isBoolean(v) || utils.none(v),
  isAdmin: (v) => _.isBoolean(v) || utils.none(v),
  measurableId: (v) => _.isBoolean(v) || utils.none(v),
  measuredByAgentId: (v) => _.isString(v) || utils.none(v),
  userId: (v) => _.isString(v) || utils.none(v),

  withinJoinedChannels: (v) => _.isObject(v) || utils.none(v),
  withinMeasurables: (v) => _.isObject(v) || utils.none(v),
  withinPublicAndJoinedChannels: (v) => _.isObject(v) || utils.none(v),
  withinPublicChannels: (v) => _.isObject(v) || utils.none(v),
};

Restrictions.LIST = list;
Restrictions.KEYS = Object.keys(list);

module.exports = {
  Restrictions,
};
