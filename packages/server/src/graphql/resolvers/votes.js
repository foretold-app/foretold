const _ = require('lodash');
const { VotesData } = require('../../data');

/**
 * @param {*} _root
 * @param {object} args
 * @param {object} args.input
 * @param {number} args.input.voteAmount
 * @param {Models.MeasurementID} args.measurementId
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function vote(_root, args, context, _info) {
  const agentId = _.get(context, 'agent.id', null);
  const measurementId = _.get(args, 'measurementId', null);
  const voteAmount = _.get(args, 'input.voteAmount', null);
  return new VotesData().vote(agentId, measurementId, voteAmount);
}

/**
 * @param {*} root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function total(root, _args, _context, _info) {
  const measurementId = _.get(root, 'id', null);
  return new VotesData().totalVoteAmount(measurementId);
}

module.exports = {
  vote,
  total,
};
