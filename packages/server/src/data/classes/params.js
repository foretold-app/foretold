const utils = require('../../lib/utils');

/**
 * See "filter.js" comments.
 */
class Params {
  /**
   * @param {Layers.DataSourceLayer.params} [params]
   */
  constructor(params = {}) {
    const list = [
      'id',
      'agentId',
      'name',
      'auth0Id',
      'measurableId',
      'competitorType',
      'seriesId',
      'email',
      'channelId',
    ];
    utils.extend(this.constructor.name, params, list, this);
    utils.diff(this.constructor.name, params, list);
  }

  inspect() {
    utils.inspect(this);
  }
}

module.exports = {
  Params,
};
