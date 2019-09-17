const _ = require('lodash');

const data = require('../../data');

const { Pagination } = require('../../data/classes/pagination');
const { Filter } = require('../../data/classes/filter');
const { Options } = require('../../data/classes/options');

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.channelId
 * @param {number} args.minNumberOfPredictions
 * @param {number} args.minNumberOfQuestionsScored
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function all(root, args, context, info) {
  const channelId = _.get(args, 'channelId');
  const currentAgentId = _.get(context, 'agent.id');
  const minNumberOfPredictions = _.get(args, 'minNumberOfPredictions');
  const minNumberOfQuestionsScored = _.get(args, 'minNumberOfQuestionsScored');

  const filter = new Filter({
    channelId,
    minNumberOfPredictions,
    minNumberOfQuestionsScored
  });
  const pagination = new Pagination(args);
  const options = new Options({ currentAgentId });

  return data.agentChannels.getConnection(filter, pagination, options);
}

/**
 * @param {*} root
 * @param {Models.Agent.id} root.agentId
 * @param {Models.Channel.id} root.channelId
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function primaryPointScore(root, args, context, info) {
  const agentId = _.get(root, 'agentId');
  const channelId = _.get(root, 'channelId');
  return data.agentChannels.primaryPointScore(agentId, channelId);
}

module.exports = {
  all,
  primaryPointScore,
};
