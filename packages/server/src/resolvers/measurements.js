const _ = require('lodash');

const data = require('../data');

const { Pagination } = require('../data/classes/pagination');
const { Filter } = require('../data/classes/filter');
const { Options } = require('../data/classes/options');
const { Params } = require('../data/classes/params');
const { Query } = require('../data/classes/query');

const { withinMeasurables } = require('../structures');

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.measurableId
 * @param {Models.ObjectID} args.agentId
 * @param {Models.ObjectID} args.notTaggedByAgent
 * @param {Models.ObjectID} args.channelId
 * @param {object} args.findInDateRange
 * @param {string} args.findInDateRange.startDate
 * @param {string} args.findInDateRange.endDate
 * @param {number} args.findInDateRange.spacedLimit
 * @param {string[]} args.competitorType
 * @param {string[]} args.measurableState
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*>}
 */
const filterGeneric = (root, args, _context, _info) => {
  const measurableState = _.get(args, 'measurableState');
  const channelId = _.get(args, 'channelId');

  return new Filter({
    withinMeasurables: withinMeasurables(measurableState, channelId),
    measurableId: _.get(args, 'measurableId'),
    agentId: _.get(args, 'agentId') || _.get(root, 'id'),
    competitorType: _.get(args, 'competitorType'),
    findInDateRange: _.get(args, 'findInDateRange'),
    notTaggedByAgent: _.get(args, 'notTaggedByAgent'),
  });
};

/**
 * @param {*} root
 * @param {*} root.measurableId
 * @param {*} root.agentId
 * @param {object} args
 * @param {string[]} args.competitorType
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
const filterA = (root, args, context, info) => {
  const agentId = _.get(root, 'agentId');
  const measurableId = _.get(root, 'measurableId');
  const competitorType = _.get(args, 'competitorType');
  return new Filter({ measurableId, agentId, competitorType });
};

/**
 * @param {*} root
 * @param {*} root.measurableId
 * @param {object} args
 * @param {string[]} args.competitorType
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
const filterB = (root, args, context, info) => {
  const measurableId = _.get(root, 'measurableId');
  const competitorType = _.get(args, 'competitorType');
  return new Filter({ measurableId, competitorType });
};

/**
 * @param {*} root
 * @param {object} args
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Pagination}
 */
const paginationGeneric = (root, args, context, info) => {
  return new Pagination(args);
};

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Pagination}
 */
const paginationFirst = (root, args, context, info) => {
  return new Pagination({ first: 1 });
};

/**
 * @param {*} result
 * @returns {*}
 */
const resultGeneric = result => result;

/**
 * @param {ResponseAll} result
 * @returns {*}
 */
const resultFirst = result => result.getFirst();

/**
 * @param {object} predicates
 * @returns {function(*=, *=, *=, *=): Promise<{data: Models.Model[], total: number}>}
 */
function all$(predicates = {
  filter: filterGeneric,
  pagination: paginationGeneric,
  result: resultGeneric
}) {
  return async function all(root, args, context, info) {
    const filter = predicates.filter(root, args, context, info);
    const pagination = predicates.pagination(root, args, context, info);
    const options = new Options({
      isAdmin: _.get(context, 'agent.isAdmin'),
      agentId: _.get(context, 'agent.id'),
    });

    const result = await data.measurements.getConnection(
      filter,
      pagination,
      options,
    );

    return predicates.result(result);
  };
}

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
const all = all$();

/**
 * @param {*} root
 * @param {object} args
 * @param {number} args.last
 * @param {number} args.first
 * @param {string[]} args.isLinkedWithAgent
 * @param {string[]} args.competitorType
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
const firstA = all$({
  filter: filterA,
  pagination: paginationFirst,
  result: resultFirst,
});

/**
 * @param {*} root
 * @param {object} args
 * @param {number} args.last
 * @param {number} args.first
 * @param {string[]} args.isLinkedWithAgent
 * @param {string[]} args.competitorType
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
const firstB = all$({
  filter: filterB,
  pagination: paginationFirst,
  result: resultFirst,
});

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
 * @param {*} root
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
 * @param {*} root
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
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function previousAggregate(root, args, context, info) {
  const measurableId = _.get(root, 'measurableId');
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
  return 0.3;
}

module.exports = {
  one,
  all,
  create,
  latest,
  scoreSet,
  prediction,
  outcome,
  previousAggregate,
  primaryPointScore,
  firstA,
  firstB,
};
