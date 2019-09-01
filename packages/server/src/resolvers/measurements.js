const _ = require('lodash');

const data = require('../data');

const { Pagination } = require('../data/classes/pagination');
const { Filter } = require('../data/classes/filter');
const { Options } = require('../data/classes/options');
const { Params } = require('../data/classes/params');
const { Query } = require('../data/classes/query');
const { Cdf, scoringFunctions } = require('@foretold/cdf');

const { withinMeasurables } = require('../structures');

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

  const result = await data.measurements.getConnection(filter, pagination, options);
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
  const creator = context.creator;
  const agentId =
    _.get(args, 'input.agentId') ||
    _.get(context, 'agent.id');
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
  return {
    id: _.get(root, 'id'),
    measurableId: _.get(root, 'measurableId'),
  };
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
 * @todo: rename to "previousAggregateByRootMeasurableId"
 * @param {object} root
 * @param {Models.ObjectID} root.measurableId
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function previousAggregate(root, args, context, info) {
  const measurableId = _.get(root, 'measurableId');
  return data.measurements.getPreviousAggregate(measurableId);
}

function getDataType(measurement){
  return measurement.dataValues.value.dataType;
}

function dataValue(measurement){
  return measurement.dataValues.value.data;
}

function matcher({prediction, aggregate, outcome}){
  if (prediction === "percentage" && aggregate === "percentage" && outcome === "binary"){
    return "percentage"
  } else if (prediction === "floatCdf" && aggregate === "floatCdf" && outcome === "floatCdf"){
    return "floatCdf"
  } else if (prediction === "floatCdf" && aggregate === "floatCdf" && outcome === "floatPoint"){
    return "floatPoint"
  } else {
    return "invalid"
  }
}

function measurementScore({prediction, aggregate, outcome}){
  // It should really return null or false if one of these doesn't exist.
  if (!prediction || !aggregate || !outcome){ return 0 };
  let combinationType = matcher({
    prediction: getDataType(prediction),
    aggregate: getDataType(aggregate),
    outcome: getDataType(outcome)
  });
  switch(combinationType){
    case "percentage":
      return scoringFunctions.percentageInputPercentageOutput({
        predictionPercentage: dataValue(prediction),
        aggregatePercentage: dataValue(aggregate),
        resultPercentage: !!dataValue(outcome) ? 100.0 : 0.0
      })
      break;
    case "floatCdf":
      return scoringFunctions.distributionInputDistributionOutput({
        predictionCdf: new Cdf(dataValue(prediction).xs, dataValue(prediction).ys),
        aggregateCdf: new Cdf(dataValue(aggregate).xs, dataValue(aggregate).ys),
        resultCdf: new Cdf(dataValue(outcome).xs, dataValue(outcome).ys)
      })
      break;
    case "floatPoint":
      return scoringFunctions.distributionInputPointOutput({
        predictionCdf: new Cdf(dataValue(prediction).xs, dataValue(prediction).ys),
        aggregateCdf: new Cdf(dataValue(aggregate).xs, dataValue(aggregate).ys),
        resultPoint: dataValue(outcome)
      })
      break;
    case "invalid":
    // It should really return null or false if invalid.
      return 0
      break;
  }
}

/**
 * @param {object} root
 * @param {Models.ObjectID} root.id
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function previousAggregateByRootId(root, _args, _context, _info) {
  const measurableId = _.get(root, 'id');
  return data.measurements.getPreviousAggregate(measurableId);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function primaryPointScore(root, args, context, info) {
  const _prediction = await prediction(root, args, context, info);
  const _previousAggregate = await previousAggregate(root, args, context, info);
  const _outcome = await outcome(root, args, context, info);
  let combinationType = {prediction: (_prediction), aggregate:(_previousAggregate), outcome: (_outcome)};
  let score = measurementScore(combinationType);
  return score;
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
  previousAggregate,
  previousAggregateByRootId,
  primaryPointScore,
  measurableMeasurement,
};
