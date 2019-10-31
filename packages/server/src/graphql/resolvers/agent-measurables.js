const _ = require('lodash');

const data = require('../../data');

const {
  MARKET_TYPE,
  START_AT,
  FINAL_COMPARISON_MEASUREMENT,
} = require('../../enums/agent-measurable-score-type');

const {
  Pagination,
  structures,
  Filter,
  Options,
} = require('../../data/classes');

/**
 * @param {*} _root
 * @param {object} args
 * @param {Models.Channel.id} args.channelId
 * @param {Models.Measurable.id} args.measurableId
 * @param {string[]} args.measurableState
 * @param {number} args.minPredictionCountTotal
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Layers.orderList} args.order
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function all(_root, args, context, _info) {
  const channelId = _.get(args, 'channelId');
  const measurableId = _.get(args, 'measurableId');
  const measurableState = _.get(args, 'measurableState');
  const currentAgentId = _.get(context, 'agent.id');
  const minPredictionCountTotal = _.get(args, 'minPredictionCountTotal');

  const withinMeasurables = structures.withinMeasurables(
    measurableState,
    channelId,
  );

  const filter = new Filter({
    withinMeasurables,
    minPredictionCountTotal,
    measurableId,
  });
  const pagination = new Pagination(args);
  const options = new Options({ currentAgentId });

  return data.agentMeasurables.getConnection(filter, pagination, options);
}

/**
 * @param {*} root
 * @param {Models.Agent.id} root.agentId
 * @param {Models.Measurable.id} root.measurableId
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function primaryPointScore(root, _args, _context, _info) {
  const agentId = _.get(root, 'agentId');
  const measurableId = _.get(root, 'measurableId');
  let response = await data.agentMeasurables.primaryPointScore(agentId, measurableId, {
    marketType: MARKET_TYPE.MARKET,
    startAt: START_AT.QUESTION_CREATION_TIME,
    finalComparisonMeasurement: FINAL_COMPARISON_MEASUREMENT.LAST_OBJECTIVE_MEASUREMENT,
  })
  if (response){
    return response.score;
  } else {
    return response;
  }
}

/**
 * @param {*} root
 * @param {Models.Agent.id} root.agentId
 * @param {Models.Measurable.id} root.measurableId
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function timeAverageScore(root, args, _context, _info) {
  const agentId = _.get(root, 'agentId');
  const measurableId = _.get(root, 'measurableId');
  const marketType = _.get(args, 'marketType');
  const startAt = _.get(args, 'startAt');
  const finalComparisonMeasurement = _.get(args, 'finalComparisonMeasurement');
  const params = { startAt, marketType, finalComparisonMeasurement };
  return data.agentMeasurables.primaryPointScore(agentId, measurableId, params);
}

module.exports = {
  all,
  primaryPointScore,
  timeAverageScore,
};
