const _ = require('lodash');
const utils = require('../../lib/utils');

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
    const list = [
      'id',
      'isArchived', // string[] | null
      'withinJoinedChannels', // Layers.withinJoinedChannels | null
      'excludeChannelId',
      'notificationId', // string
      'sentAt', // Date | null
      'attemptCounterMax', // number
      'types', // string[] | null
      'type',
      'creatorId',
      'userId',
      'seriesId', // string
      'channelId', // string
      'measurableId', // string
      'agentId',
      'competitorType', // string[]
      'findInDateRange', // object
      'notTaggedByAgent', // string | null
      'states', // string[] | null
      'measurableState', // string[] | null
    ];
    utils.extend(this.constructor.name, filter, list, this);
    utils.diff(this.constructor.name, filter, list);
  }

  /**
   * @public
   * @returns {any}
   */
  getSpacedLimit() {
    return _.get(this, 'findInDateRange.spacedLimit');
  }

  /**
   * @public
   */
  inspect() {
    utils.inspect(this);
  }
}

module.exports = {
  Filter,
};
