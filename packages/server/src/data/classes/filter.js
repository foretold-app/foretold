const _ = require('lodash');

/**
 * This class is used as a simple
 * constructor to make interface inputs
 * of "Data Layer" methods. The main purpose is
 * to avoid not meaningfully literals of objects
 * as "{}". So in the tuple "({}, {}, {})" you cannot
 * understand what exactly each of argument is. But
 * if you will use annotation classes
 * (new Filter(), new Pagination(). new Options())
 * the tuple would become more readable.
 *
 * Usage example:
 * const filter = new Filter({agentId: "someId"});
 * new NotificationDate().getAll(filter);
 *
 */

class Filter {
  /**
   * @param {Layers.DataSourceLayer.filter} [filter]
   */
  constructor(filter = {}) {
    const common = ['id', 'isArchived'];
    const channel = [];
    const agent = ['excludeChannelId', 'types'];
    const agentNotification = [
      'notificationId', // string
      'sentAt', // Date | null
      'attemptCounterMax', // number
    ];
    const channels = ['channelMemberId'];

    const list = [
      'type', 'creatorId', ' userId',
      'seriesId', 'channelId', 'measurableId',
      'agentId', 'competitorType',
      'findInDateRange', 'notTaggedByAgent', 'states',
      ...common,
      ...channel,
      ...channels,
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
