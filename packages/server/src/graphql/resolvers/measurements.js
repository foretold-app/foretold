const _ = require('lodash');
const { Cdf } = require('@foretold/cdf');

const {
  PredictionResolutionGroup,
  marketScore,
  nonMarketScore,
} = require('@foretold/prediction-analysis');

const { MeasurementsData } = require('../../data');
const { withinMeasurables } = require('../../data/classes/structures');

const { Pagination } = require('../../data/classes');
const { Filter } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');

const { MEASUREMENT_COMPETITOR_TYPE } = require('../../enums');
const logger = require('../../lib/log');

const log = logger.module('resolvers/measurements');

/**
 * @param {*} root
 * @param {object} args
 * @param {number} args.last
 * @param {number} args.first
 * @param {Models.MeasurableID} args.measurableId
 * @param {Models.AgentID} args.agentId
 * @param {Models.AgentID} args.notTaggedByAgent
 * @param {Models.ChannelID} args.channelId
 *
 * @param {object} args.findInDateRange
 * @param {string} args.findInDateRange.startDate
 * @param {string} args.findInDateRange.endDate
 * @param {number} args.findInDateRange.spacedLimit
 *
 * @param {string[]} args.competitorType
 * @param {string[]} args.measurableState
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function all(root, args, context, _info) {
  const measurableState = _.get(args, 'measurableState', null);
  const channelId = _.get(args, 'channelId', null);

  const filter = new Filter({
    withinMeasurables: withinMeasurables(measurableState, channelId),
    measurableId: _.get(args, 'measurableId', null),
    agentId: _.get(args, 'agentId', null) || _.get(root, 'id', null),
    competitorType: _.get(args, 'competitorType', null),
    findInDateRange: _.get(args, 'findInDateRange', null),
    notTaggedByAgent: _.get(args, 'notTaggedByAgent', null),
  });
  const pagination = new Pagination(args);
  const options = new Options({
    isAdmin: _.get(context, 'agent.isAdmin', null),
    agentId: _.get(context, 'agent.id', null),
  });

  return new MeasurementsData().getConnection(filter, pagination, options);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {number} args.last
 * @param {number} args.first
 * @param {string[]} args.competitorType
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function measurableMeasurement(root, args, context, _info) {
  const measurableId = _.get(root, 'measurableId', null);
  const competitorType = _.get(args, 'competitorType', null);

  const filter = new Filter({ measurableId, competitorType });
  const pagination = new Pagination({ first: 1 });
  const options = new Options({
    isAdmin: _.get(context, 'agent.isAdmin', null),
    agentId: _.get(context, 'agent.id', null),
  });

  const result = await new MeasurementsData().getConnection(
    filter,
    pagination,
    options,
  );

  return result.getFirst();
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.MeasurementID} args.id
 * @param {string[]} args.competitorType
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function one(root, args, context, _info) {
  const id = _.get(args, 'id', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const params = new Params({ id });
  const query = new Query();
  const options = new Options({
    isAdmin: _.get(context, 'agent.isAdmin', null),
    agentId: currentAgentId,
  });

  return new MeasurementsData().getOne(params, query, options);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function create(root, args, context, _info) {
  const agentId = _.get(args, 'input.agentId', null)
    || _.get(context, 'agent.id', null);

  const data = { ...args.input, agentId };

  return new MeasurementsData().createOne(data);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function latest(root, args, context, _info) {
  const measurable = root;
  const agentId = _.get(args, 'input.agentId', null)
    || _.get(context, 'agent.id', null);

  if (!measurable) return null;
  if (!agentId) return null;

  return new MeasurementsData().getLatest({
    measurable,
    agentId,
  });
}

/**
 * I feel something strange doing it.
 * The time will show.
 * @param {*} root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function scoreSet(root, _args, _context, _info) {
  return root;
}

/**
 * Do not optimize.
 * Do not return "root".
 * @todo: rename to "predictionByRootId"
 * @param {object} root
 * @param {Models.MeasurementID} root.id
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function prediction(root, _args, _context, _info) {
  const id = _.get(root, 'id', null);
  const params = new Params({ id });
  return new MeasurementsData().getOne(params);
}

/**
 * @todo: rename to "outcomeByRootMeasurableId"
 * @param {object} root
 * @param {Models.MeasurableID} root.measurableId
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function outcome(root, _args, _context, _info) {
  const measurableId = _.get(root, 'measurableId', null);
  return new MeasurementsData().getOutcome(measurableId);
}

/**
 * @param {object} root
 * @param {Models.MeasurementID} root.id
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function outcomeByRootId(root, _args, _context, _info) {
  const measurableId = _.get(root, 'id', null);
  return new MeasurementsData().getOutcome(measurableId);
}

/**
 * @param {object} root
 * @param {Models.MeasurableID} root.measurableId
 * @param {Date} root.createdAt
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function previousAggregate(root, args, context, _info) {
  const measurableId = _.get(root, 'measurableId', null);
  const createdAt = _.get(root, 'createdAt', null);
  const agentId = _.get(context, 'botAgentId', null);

  return new MeasurementsData().getPreviousRelevantAggregate(
    measurableId,
    agentId,
    createdAt,
  );
}

/**
 * @param {object} root
 * @param {Models.MeasurementID} root.id
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function latestAggregateByRootId(root, _args, context, _info) {
  const measurableId = _.get(root, 'id', null);
  const agentId = _.get(context, 'botAgentId', null);

  return new MeasurementsData().getLatestAggregate(measurableId, agentId);
}

/**
 * @todo: duplicated.
 * @param r
 * @returns {{data: *, dataType: *}}
 */
function translateValue(r) {
  let { data } = r.value;
  const { dataType } = r.value;
  if (dataType === 'percentage') {
    data /= 100;
  }
  return { data, dataType };
}

/**
 * @param prediction
 * @param aggregate
 * @param outcome
 * @returns {number|*}
 */
function _marketLogScore({ prediction, aggregate, outcome }) {
  if (!prediction || !aggregate || !outcome) {
    return { error: 'MeasurementScore Error: Missing needed data' };
  }

  const { competitorType } = prediction;

  if (
    competitorType !== MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE
  ) {
    return { error: 'MeasurementScore Exception: Measurement not competitive' };
  }

  return new PredictionResolutionGroup({
    agentPrediction: translateValue(prediction),
    marketPrediction: translateValue(aggregate),
    resolution: translateValue(outcome),
  }).pointScore(marketScore);
}

/**
 * @param prediction
 * @param aggregate
 * @param outcome
 * @returns {number|*}
 */
function _nonMarketLogScore({ prediction, outcome }) {
  if (!prediction || !outcome) {
    return { error: '_nonMarketLogScore Error: Missing needed data' };
  }

  const { competitorType } = prediction;

  if (
    competitorType !== MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE
    && competitorType !== MEASUREMENT_COMPETITOR_TYPE.AGGREGATION
  ) {
    return { error: 'False' };
  }

  return new PredictionResolutionGroup({
    agentPrediction: translateValue(prediction),
    marketPrediction: undefined,
    resolution: translateValue(outcome),
  }).pointScore(nonMarketScore);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function primaryPointScore(root, args, context, info) {
  const result = _marketLogScore({
    prediction: await prediction(root, args, context, info),
    aggregate: await previousAggregate(root, args, context, info),
    outcome: await outcome(root, args, context, info),
  });

  if (result.error) {
    log.trace('ERROR: ', result.error);
    return undefined;
  }

  return _.isFinite(result.data) ? _.round(result.data, 6) : undefined;
}

/**
 * @param {*} root
 * @param {string} root.competitorType
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<number>}
 */
async function nonMarketLogScore(root, args, context, info) {
  const result = _nonMarketLogScore({
    prediction: await prediction(root, args, context, info),
    outcome: await outcome(root, args, context, info),
  });

  if (result.error) {
    log.trace('ERROR: ', result.error);
    return undefined;
  }

  return _.isFinite(result.data) ? _.round(result.data, 6) : undefined;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function truncateCdf(root, args, _context, _info) {
  if (!_.get(root, 'floatCdf')) return null;

  const truncate = _.get(args, 'truncate');
  const round = _.get(args, 'round');

  const xs = _.get(root, 'floatCdf.xs');
  const ys = _.get(root, 'floatCdf.ys');
  let result = { xs, ys };

  if (truncate !== null && truncate !== undefined) {
    const resized = new Cdf(xs, ys).convertToNewLength(truncate);
    result = { xs: resized.xs, ys: resized.ys };
  }

  if (round !== null && round !== undefined) {
    result.xs = result.xs.map((i) => _.round(i, round));
    result.ys = result.ys.map((i) => _.round(i, round));
  }

  return result;
}

/**
 * @param {object} root
 * @param {Models.MeasurementID} root.id
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function measurementCountByAgentId(root, _args, _context, _info) {
  const agentId = _.get(root, 'id', null);
  return new MeasurementsData().getCount(new Params({ agentId }));
}

/**
 * @param {object} root
 * @param {Models.MeasurementID} root.id
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function measurementCountByMeasurableId(root, _args, _context, _info) {
  const measurableId = _.get(root, 'id', null);
  return new MeasurementsData().getCount(new Params({ measurableId }));
}

/**
 * @param {object} _root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function count(_root, _args, _context, _info) {
  return new MeasurementsData().getCount();
}

/**
 * @param {object} root
 * @param {Models.MeasurementID} root.id
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function measurerCount(root, _args, _context, _info) {
  const measurableId = _.get(root, 'id', null);
  return new MeasurementsData().getCount(new Params({
    measurableId,
  }), new Query({
    distinct: true,
    col: 'agentId',
  }));
}

module.exports = {
  one,
  all,
  count,
  create,
  latest,
  scoreSet,
  prediction,
  outcome,
  outcomeByRootId,
  nonMarketLogScore,
  previousAggregate,
  latestAggregateByRootId,
  primaryPointScore,
  measurableMeasurement,
  truncateCdf,
  measurementCountByAgentId,
  measurementCountByMeasurableId,
  measurerCount,
};
