const _ = require('lodash');

class Filter {
  /**
   * @param {Layers.DataSourceLayer.filter} [filter]
   */
  constructor(filter = {}) {
    const common = ['id'];
    const channel = ['isArchived'];
    const agent = ['excludeChannelId', 'types'];
    const agentNotification = ['notificationId', 'sentAt'];

    const list = [
      'type', 'creatorId', ' userId',
      'seriesId', 'channelId', 'measurableId',
      'agentId', 'competitorType',
      'findInDateRange', 'notTaggedByAgent', 'states',
      ...common,
      ...channel,
      ...agent,
      ...agentNotification,
    ];

    _.each(list, (name) => {
      if (_.has(filter, name)) {
        this[name] = _.get(filter, name);
      }
    });
  }
}

module.exports = {
  Filter,
};
