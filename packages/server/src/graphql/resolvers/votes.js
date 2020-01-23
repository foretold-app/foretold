const _ = require('lodash');
const { VotesData } = require('../../data');

/**
 * @param {*} _root
 * @param {object} args
 * @param {Models.MeasurementID} args.measurementId
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function upvote(_root, args, context, _info) {
  const agentId = _.get(context, 'agent.id', null);
  const measurementId = _.get(args, 'measurementId', null);
  return new VotesData().upvote(agentId, measurementId);
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Models.MeasurementID} args.measurementId
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function downvote(_root, args, context, _info) {
  const agentId = _.get(context, 'agent.id', null);
  const measurementId = _.get(args, 'measurementId', null);
  return new VotesData().downvote(agentId, measurementId);
}

module.exports = {
  upvote,
  downvote,
};
