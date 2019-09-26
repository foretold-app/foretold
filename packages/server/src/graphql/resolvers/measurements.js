const _ = require('lodash');
const { Cdf } = require('@foretold/cdf');

const {
  PredictionResolutionGroup,
  marketScore,
  nonMarketScore,
} = require('@foretold/prediction-analysis');

const data = require('../../data');
const { withinMeasurables } = require('../../structures');

const { Pagination } = require('../../data/classes/pagination');
const { Filter } = require('../../data/classes/filter');
const { Options } = require('../../data/classes/options');
const { Params } = require('../../data/classes/params');
const { Query } = require('../../data/classes/query');
const { MEASUREMENT_COMPETITOR_TYPE } = require('../../enums/measurement-competitor-type');

/**
 * @param {*} root
 * @param {object} args
 * @param {number} args.last
 * @param {number} args.first
 * @param {Models.ObjectID} args.measurableId
 * @param {Models.ObjectID} args.agentId
 * @param {Models.ObjectID} args.notTaggedByAgent
 * @param {Models.ObjectID} args.channelId
 *
 * @param {object} args.findInDateRange
 * @param {string} args.findInDateRange.startDate
 * @param {string} args.findInDateRange.endDate
 * @param {number} args.findInDateRange.spacedLimit
 *
 * @param {string[]} args.competitorType
 * @param {string[]} args.measurableState
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function all(root, args, context, info) {
  const measurableState = _.get(args, 'measurableState');
  const channelId = _.get(args, 'channelId');

  const filter = new Filter({
    withinMeasurables: withinMeasurables(measurableState, channelId),
    measurableId: _.get(args, 'measurableId'),
    agentId: _.get(args, 'agentId') || _.get(root, 'id'),
    competitorType: _.get(args, 'competitorType'),
    findInDateRange: _.get(args, 'findInDateRange'),
    notTaggedByAgent: _.get(args, 'notTaggedByAgent'),
  });
  const pagination = new Pagination(args);
  const options = new Options({
    isAdmin: _.get(context, 'agent.isAdmin'),
    agentId: _.get(context, 'agent.id'),
  });

  return data.measurements.getConnection(filter, pagination, options);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {number} args.last
 * @param {number} args.first
 * @param {string[]} args.competitorType
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function measurableMeasurement(root, args, context, info) {
  const measurableId = _.get(root, 'measurableId');
  const competitorType = _.get(args, 'competitorType');

  const filter = new Filter({ measurableId, competitorType });
  const pagination = new Pagination({ first: 1 });
  const options = new Options({
    isAdmin: _.get(context, 'agent.isAdmin'),
    agentId: _.get(context, 'agent.id'),
  });

  const result = await data.measurements.getConnection(
    filter,
    pagination,
    options,
  );
  return result.getFirst();
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {string[]} args.competitorType
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function one(root, args, context, info) {
  const id = _.get(args, 'id');
  const currentAgentId = _.get(context, 'agent.id');

  const params = new Params({ id });
  const query = new Query();
  const options = new Options({
    isAdmin: _.get(context, 'agent.isAdmin'),
    agentId: currentAgentId,
  });

  return data.measurements.getOne(params, query, options);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function create(root, args, context, info) {
  const { creator } = context;
  const agentId = _.get(args, 'input.agentId')
    || _.get(context, 'agent.id');
  const datas = { ...args.input, agentId };
  return data.measurements.createOne(datas, creator);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function latest(root, args, context, info) {
  const measurable = root;
  const agentId = context.resultOrLatestMeasurementForAgentId;
  if (!measurable) return null;
  if (!agentId) return null;
  return data.measurements.getLatest({
    measurable,
    agentId,
  });
}

/**
 * I feel something strange doing it.
 * The time will show.
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function scoreSet(root, args, context, info) {
  return root;
}

/**
 * Do not optimize.
 * Do not return "root".
 * @todo: rename to "predictionByRootId"
 * @param {object} root
 * @param {Models.ObjectID} root.id
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function prediction(root, args, context, info) {
  const id = _.get(root, 'id');
  const params = new Params({ id });
  return data.measurements.getOne(params);
}

/**
 * @todo: rename to "outcomeByRootMeasurableId"
 * @param {object} root
 * @param {Models.ObjectID} root.measurableId
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function outcome(root, args, context, info) {
  const measurableId = _.get(root, 'measurableId');
  return data.measurements.getOutcome(measurableId);
}

/**
 * @param {object} root
 * @param {Models.ObjectID} root.id
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function outcomeByRootId(root, _args, _context, _info) {
  const measurableId = _.get(root, 'id');
  return data.measurements.getOutcome(measurableId);
}

/**
 * @param {object} root
 * @param {Models.ObjectID} root.measurableId
 * @param {Date} root.createdAt
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function previousAggregate(root, args, context, info) {
  const measurableId = _.get(root, 'measurableId');
  const createdAt = _.get(root, 'createdAt');
  const agentId = _.get(context, 'botAgentId');
  return data.measurements.getPreviousRelevantAggregate(
    measurableId,
    agentId,
    createdAt,
  );
}

/**
 * @param {object} root
 * @param {Models.ObjectID} root.id
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function latestAggregateByRootId(root, _args, context, _info) {
  const measurableId = _.get(root, 'id');
  const agentId = _.get(context, 'botAgentId');
  return data.measurements.getLatestAggregate(measurableId, agentId);
}

/**
 * @param r
 * @returns {{data: *, dataType: *}}
 */
function translateValue(r) {
  let { data, dataType } = r.dataValues.value;
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
    return ({ error: 'MeasurementScore Error: Missing needed data' });
  }

  const { competitorType } = prediction.dataValues;

  if (
    competitorType !== MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE
  ) {
    return ({ error: 'False' });
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
    return ({ error: '_nonMarketLogScore Error: Missing needed data' });
  }

  const { competitorType } = prediction.dataValues;

  if (
    competitorType !== MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE
    && competitorType !== MEASUREMENT_COMPETITOR_TYPE.AGGREGATION
  ) {
    return ({ error: 'False' });
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
    console.log('ERROR: ', result.error);
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
    console.log('ERROR: ', result.error);
    return undefined;
  }

  return _.isFinite(result.data) ? _.round(result.data, 6) : undefined;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function truncateCdf(root, args, context, info) {
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

module.exports = {
  one,
  all,
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
};
