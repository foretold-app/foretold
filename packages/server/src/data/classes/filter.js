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
/**
 * @implements {Layers.Filter}
 */
class Filter {
  /**
   * @param {object} [filter]
   */
  constructor(filter = {}) {
    utils.extend(this.constructor.name, filter, Filter.LIST, this);
    utils.test(this.constructor.name, Filter.LIST, this);
    utils.diff(this.constructor.name, filter, Filter.LIST);
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

const list = {
  agentId: (v) => _.isString(v) || utils.none(v),
  attemptCounterMax: (v) => _.isNumber(v) || utils.none(v),
  channelId: (v) => _.isString(v) || utils.none(v),
  competitorType: (v) => _.isArray(v) || utils.none(v),
  creatorId: (v) => _.isString(v) || utils.none(v),
  email: (v) => _.isString(v) || utils.none(v),
  excludeChannelId: (v) => _.isString(v) || utils.none(v),
  findInDateRange: (v) => _.isObject(v) || utils.none(v),
  id: (v) => _.isString(v) || utils.none(v),
  isArchived: (v) => _.isArray(v) || utils.none(v),
  isEmailVerified: (v) => _.isArray(v) || utils.none(v),
  isNotEmailVerified: (v) => _.isBoolean(v) || utils.none(v),
  isVerified: (v) => _.isBoolean(v) || utils.none(v),
  isCancelled: (v) => _.isBoolean(v) || utils.none(v),
  measurableId: (v) => _.isString(v) || utils.none(v),
  measurableIds: (v) => _.isArray(v) || utils.none(v),
  measuredByAgentId: (v) => _.isString(v) || utils.none(v),
  measurementId: (v) => _.isString(v) || utils.none(v),
  minNumberOfPredictions: (v) => _.isNumber(v) || utils.none(v),
  minNumberOfQuestionsScored: (v) => _.isNumber(v) || utils.none(v),
  minPredictionCountTotal: (v) => _.isNumber(v) || utils.none(v),
  needsResolutionResponse: (v) => _.isBoolean(v) || utils.none(v),
  needsToBePending: (v) => _.isBoolean(v) || utils.none(v),
  notAuth0AccessToken: (v) => _.isBoolean(v) || utils.none(v),
  notTaggedByAgent: (v) => _.isString(v) || utils.none(v),
  notificationId: (v) => _.isString(v) || utils.none(v),
  ownerId: (v) => _.isString(v) || utils.none(v),
  role: (v) => _.isString(v) || utils.none(v),
  sentAt: (v) => _.isString(v) || _.isObject(v) || utils.none(v),
  seriesId: (v) => _.isString(v) || utils.none(v),
  states: (v) => _.isArray(v) || utils.none(v),
  status: (v) => _.isString(v) || utils.none(v),
  taggedMeasurementId: (v) => _.isObject(v) || utils.none(v),
  type: (v) => _.isString(v) || utils.none(v),
  types: (v) => _.isArray(v) || utils.none(v),
  userId: (v) => _.isString(v) || utils.none(v),
  withinJoinedChannels: (v) => _.isObject(v) || utils.none(v),
  withinMeasurables: (v) => _.isObject(v) || utils.none(v),
  labelProperty: (v) => _.isArray(v) || utils.none(v),
  labelSubject: (v) => _.isArray(v) || utils.none(v),
};

Filter.LIST = list;
Filter.KEYS = Object.keys(list);

module.exports = {
  Filter,
};
