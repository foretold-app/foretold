const _ = require('lodash');
const { VotesData } = require('../../data');

const { Params } = require('../../data/classes');
const { Filter } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Pagination } = require('../../data/classes');

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
async function measurementVote(_root, args, context, _info) {
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

/**
 * @param {*} root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.User>}
 */
async function oneByMeasurementId(root, _args, context, _info) {
  const measurementId = _.get(root, 'id', null);
  const currentAgentId = _.get(context, 'agent.id', null);
  const params = new Params({ measurementId, agentId: currentAgentId });
  return new VotesData().getOne(params);
}

/**
 * @param {object | null} _root
 * @param {object} args
 * @param {Models.MeasurementID} args.measurementId
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<Models.Channel[]>}
 */
async function all(_root, args, _context, _info) {
  const measurementId = _.get(args, 'measurementId', null);

  const filter = new Filter({ measurementId });
  const pagination = new Pagination(args);
  const options = new Options({ raw: true });

  const response = await new VotesData().getConnection(
    filter, pagination, options,
  );

  return response.getData();
}

module.exports = {
  all,
  measurementVote,
  oneByMeasurementId,
  total,
};
