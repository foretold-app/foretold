const _ = require('lodash');

const { AgentChannelsData } = require('../../data');

const { Pagination } = require('../../data/classes');
const { Filter } = require('../../data/classes');
const { Options } = require('../../data/classes');

/**
 * @param {*} root
 * @param {object} args
 * @param {Defs.ChannelID} args.channelId
 * @param {Defs.AgentID} args.agentId
 * @param {number} args.minNumberOfPredictions
 * @param {number} args.minNumberOfQuestionsScored
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function all(root, args, context, _info) {
  const channelId = _.get(args, 'channelId', null);
  const agentId = _.get(args, 'agentId', null);
  const currentAgentId = _.get(context, 'agent.id', null);
  const minNumberOfPredictions = _.get(args, 'minNumberOfPredictions', null);
  const minNumberOfQuestionsScored = _.get(
    args,
    'minNumberOfQuestionsScored',
    null,
  );

  const filter = new Filter({
    channelId,
    agentId,
    minNumberOfPredictions,
    minNumberOfQuestionsScored,
  });
  const pagination = new Pagination(args);
  const options = new Options({ currentAgentId, raw: true });

  return new AgentChannelsData().getConnection(filter, pagination, options);
}

/**
 * @param {*} root
 * @param {Defs.Agent.id} root.agentId
 * @param {Defs.Channel.id} root.channelId
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function primaryPointScore(root, _args, _context, _info) {
  const agentId = _.get(root, 'agentId', null);
  const channelId = _.get(root, 'channelId', null);
  return new AgentChannelsData().primaryPointScore(agentId, channelId);
}

module.exports = {
  all,
  primaryPointScore,
};
