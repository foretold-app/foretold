const _ = require('lodash');

const { FeedItemsData } = require('../../data');

const { Pagination } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Filter } = require('../../data/classes');
const { structures } = require('../../data/classes');

const { HOME_CHANNEL_ID } = require('../../../config/well-known');

/**
 *
 * Do not make wrong conclusions about "Filters"
 * and "Restrictions". "Filters" are used to filter
 * objects. However, "Restrictions" are used to
 * restrict an access to objects.
 *
 * @param {*} _root
 * @param {object} args
 * @param {Models.ChannelID} args.channelId
 * @param {Models.AgentID} args.agentId
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function all(_root, args, context, _info) {
  /** @type {Models.ChannelID | null} */
  const channelId = _.get(args, 'channelId', null);
  /** @type {Models.AgentID | null} */
  const agentId = _.get(args, 'agentId', null);
  /** @type {Models.AgentID | null} */
  const currentAgentId = _.get(context, 'agent.id', null);

  /**
   * @todo: It was the hardest mind trap. How to
   * @todo: name this piece of the code?
   * @todo: isRequesterAuthorizedAndWantsToSeeHomePage?
   * @todo: Either to create a "predicate" function
   * @todo: and leave condition here?
   * @todo: search tag: "withinJoinedChannelsByChannelId"
   * @type {Layers.withinJoinedChannels | null}
   */
  const withinJoinedChannels = channelId === HOME_CHANNEL_ID && !!currentAgentId
    ? structures.withinJoinedChannelsByChannelId(currentAgentId) : null;

  const filter = new Filter({ agentId, channelId, withinJoinedChannels });
  const pagination = new Pagination(args);
  const options = new Options({ currentAgentId, raw: true });

  return new FeedItemsData().getConnection(
    filter,
    pagination,
    options,
  );
}

module.exports = {
  all,
};
