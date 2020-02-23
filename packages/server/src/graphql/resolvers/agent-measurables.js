const _ = require('lodash');

const { AgentMeasurablesData } = require('../../data');

const {
  Pagination,
  structures,
  Filter,
  Options,
} = require('../../data/classes');

/**
 * @param {*} _root
 * @param {object} args
 * @param {Defs.Channel.id} args.channelId
 * @param {Defs.Measurable.id} args.measurableId
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
  const channelId = _.get(args, 'channelId', null);
  const measurableId = _.get(args, 'measurableId', null);
  const measurableState = _.get(args, 'measurableState', null);
  const currentAgentId = _.get(context, 'agent.id', null);
  const minPredictionCountTotal = _.get(args, 'minPredictionCountTotal', null);

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
  const options = new Options({ currentAgentId, raw: true });

  return new AgentMeasurablesData().getConnection(filter, pagination, options);
}

/**
 * @param {*} root
 * @param {Defs.Agent.id} root.agentId
 * @param {Defs.Measurable.id} root.measurableId
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function primaryPointScore(root, _args, _context, _info) {
  const agentId = _.get(root, 'agentId', null);
  const measurableId = _.get(root, 'measurableId', null);

  const response = await new AgentMeasurablesData().primaryPointScore(
    agentId,
    measurableId,
  );

  if (response) {
    return response.score;
  }
  return response;
}

/**
 * @param {*} root
 * @param {Defs.Agent.id} root.agentId
 * @param {Defs.Measurable.id} root.measurableId
 * @param {object} args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function timeAverageScore(root, args, _context, _info) {
  const agentId = _.get(root, 'agentId', null);
  const measurableId = _.get(root, 'measurableId', null);
  const marketType = _.get(args, 'marketType', null);
  const startAt = _.get(args, 'startAt', null);
  const finalComparisonMeasurement
    = _.get(args, 'finalComparisonMeasurement', null);

  const params = { startAt, marketType, finalComparisonMeasurement };

  return new AgentMeasurablesData().primaryPointScore(
    agentId,
    measurableId,
    params,
  );
}

module.exports = {
  all,
  primaryPointScore,
  timeAverageScore,
};
