const _ = require('lodash');
const { VotesData } = require('../../data');

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.MeasurableID} args.measurableId
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function upvote(root, args, context, info) {
  const agentId = _.get(context, 'agent.id', null);
  const measurableId = _.get(args, 'measurableId', null);
  return new VotesData().upvote(agentId, measurableId);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.MeasurableID} args.measurableId
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function downvote(root, args, context, info) {
  const agentId = _.get(context, 'agent.id', null);
  const measurableId = _.get(args, 'measurableId', null);
  return new VotesData().downvote(agentId, measurableId);
}

module.exports = {
  upvote,
  downvote,
};
