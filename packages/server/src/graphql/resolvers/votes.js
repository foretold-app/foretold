const _ = require('lodash');
const { VotesData } = require('../../data');

const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Options } = require('../../data/classes');

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
  upvote,
  downvote,
  total,
};
