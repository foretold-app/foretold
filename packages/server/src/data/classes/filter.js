const _ = require('lodash');

class Filter {
  /**
   * @param {Layers.DataSourceLayer.filter} [filter]
   */
  constructor(filter = {}) {
    this.id = _.get(filter, 'id');
    this.type = _.get(filter, 'type');
    this.creatorId = _.get(filter, 'creatorId');
    this.seriesId = _.get(filter, 'seriesId');
    this.channelId = _.get(filter, 'channelId');
    this.measurableId = _.get(filter, 'measurableId');
    this.agentId = _.get(filter, 'agentId');
    this.excludeChannelId = _.get(filter, 'excludeChannelId');
    this.competitorType = _.get(filter, 'competitorType');
    this.findInDateRange = _.get(filter, 'findInDateRange');
    this.notTaggedByAgent = _.get(filter, 'notTaggedByAgent');
    this.states = _.get(filter, 'states');
    this.isArchived = _.get(filter, 'isArchived');
    this.types = _.get(filter, 'types');
    this.type = _.get(filter, 'type');
    this.notificationId = _.get(filter, 'notificationId');
  }
}

module.exports = {
  Filter,
};
