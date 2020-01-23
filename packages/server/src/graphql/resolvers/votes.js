const _ = require('lodash');
const { VotesData } = require('../../data');

/**
 * @param {*} _root
 * @param {object} args
 * @param {Models.MeasurableID} args.measurableId
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function upvote(_root, args, context, _info) {
  const agentId = _.get(context, 'agent.id', null);
  const measurableId = _.get(args, 'measurableId', null);
  return new VotesData().upvote(agentId, measurableId);
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Models.MeasurableID} args.measurableId
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function downvote(_root, args, context, _info) {
  const agentId = _.get(context, 'agent.id', null);
  const measurableId = _.get(args, 'measurableId', null);
  return new VotesData().downvote(agentId, measurableId);
}

module.exports = {
  upvote,
  downvote,
};
