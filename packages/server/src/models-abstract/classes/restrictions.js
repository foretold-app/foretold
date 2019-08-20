const _ = require('lodash');
const utils = require('../../lib/utils');

class Restrictions {
  /**
   * @param {Layers.AbstractModelsLayer.restrictions} [options]
   */
  constructor(options) {
    const list = {
      'isAdmin': v => _.isBoolean(v) || _.isUndefined(v),
      'agentId': v => _.isString(v),
      'measuredByAgentId': v => _.isString(v) || _.isUndefined(v),
      'userId': v => _.isString(v),
      'channelId': v => _.isBoolean(v),
      'measurableId': v => _.isBoolean(v),
      'channelIdAsId': v => _.isBoolean(v),
      'channelMemberId': v => _.isString(v),
    };
    utils.extend(this.constructor.name, options, list, this);
    utils.test(this.constructor.name, list, this);
    utils.diff(this.constructor.name, options, list);
  }
}

module.exports = {
  Restrictions,
};
