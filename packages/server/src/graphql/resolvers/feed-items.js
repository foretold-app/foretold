const _ = require('lodash');

const data = require('../../data');

const { Pagination } = require('../../data/classes/pagination');
const { Options } = require('../../data/classes/options');
const { Filter } = require('../../data/classes/filter');

const { HOME_CHANNEL_ID } = require('../../well-known');
const structures = require('../../data/classes/structures');

/**
 * @param {*} _root
 * @param {object} args
 * @param {Models.ObjectID} args.channelId
 * @param {Models.ObjectID} args.agentId
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function all(_root, args, context, _info) {
  /** @type {string} */
  const channelId = _.get(args, 'channelId');
  /** @type {string} */
  const agentId = _.get(args, 'agentId');
  /** @type {string} */
  const currentAgentId = _.get(context, 'agent.id');

  const withinJoinedChannels = channelId === HOME_CHANNEL_ID && !!currentAgentId
    ? structures.withinJoinedChannelsByChannelId(currentAgentId) : null;

  const filter = new Filter({ agentId, channelId, withinJoinedChannels });
  const pagination = new Pagination(args);
  const options = new Options({ currentAgentId });

  return data.feedItems.getConnection(
    filter,
    pagination,
    options,
  );
}

module.exports = {
  all,
};
