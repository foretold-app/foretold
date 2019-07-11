const _ = require('lodash');

class Filter {
  /**
   * @param {Layers.DataSourceLayer.filter} [filter]
   */
  constructor(filter = {}) {
    const common = ['id'];
    const measurable = [];
    const measurement = [];
    const agentChannel = [];
    const bot = [];
    const user = [];
    const series = [];
    const template = [];
    const token = [];
    const channel = ['isArchived'];
    const agent = ['excludeChannelId', 'types'];
    const notification = [];
    const agentNotification = ['notificationId', 'sentAt'];

    const list = [
      'type', 'creatorId',
      'seriesId', 'channelId', 'measurableId',
      'agentId', 'competitorType',
      'findInDateRange', 'notTaggedByAgent', 'states',
      'userId',
      ...common,
      ...measurable,
      ...measurement,
      ...agentChannel,
      ...bot,
      ...user,
      ...series,
      ...template,
      ...token,
      ...channel,
      ...agent,
      ...notification,
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
