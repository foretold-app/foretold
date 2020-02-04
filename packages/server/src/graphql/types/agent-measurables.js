const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');
const resolvers = require('../resolvers');

const agentsTypes = require('./agents');
const measurablesTypes = require('./measurables');
const commonTypes = require('./common');
const measurementsTypes = require('./measurements');
const { measurementCompetitorType } = require('./enums');
const { marketScoreType } = require('./enums');
const { startAt } = require('./enums');
const { finalComparisonMeasurement } = require('./enums');

const points = graphql.GraphQLList(new graphql.GraphQLObjectType({
  name: 'point',
  fields: () => ({
    x: { type: graphql.GraphQLInt },
    y: { type: graphql.GraphQLFloat },
  }),
}));

const activeTimeDistribution = new graphql.GraphQLObjectType({
  name: 'timeDistribution',
  fields: () => ({
    finalX: { type: graphql.GraphQLInt },
    points: { type: points },
  }),
});

const timeAverageScore = new graphql.GraphQLObjectType({
  name: 'timeAverageScore',
  fields: () => ({
    score: { type: graphql.GraphQLFloat },
    agentPredictions: {
      type: graphql.GraphQLList(measurementsTypes.measurement),
    },
    aggregations: {
      type: graphql.GraphQLList(measurementsTypes.measurement),
    },
    recentResult: { type: measurementsTypes.measurement },
    scoringStartTime: { type: DateType.default },
    scoringEndTime: { type: DateType.default },
    measurableCreationTime: { type: DateType.default },
    finalResolutionTime: { type: DateType.default },
    timeActivityRatio: { type: graphql.GraphQLFloat },
    activeTimeDistribution: { type: activeTimeDistribution },
    totalVotes: { type: graphql.GraphQLInt },
  }),
});

const agentMeasurable = new graphql.GraphQLObjectType({
  name: 'AgentMeasurable',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    measurableId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    primaryPointScore: {
      type: graphql.GraphQLFloat,
      resolve: resolvers.agentMeasurables.primaryPointScore,
    },
    timeAverageScore: {
      type: timeAverageScore,
      resolve: resolvers.agentMeasurables.timeAverageScore,
      args: {
        marketType: { type: marketScoreType },
        startAt: { type: startAt },
        finalComparisonMeasurement: { type: finalComparisonMeasurement },
      },
    },
    predictionCountTotal: { type: graphql.GraphQLNonNull(graphql.GraphQLInt) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },

    // OK
    agent: {
      type: graphql.GraphQLNonNull(agentsTypes.agent),
      resolve: resolvers.agents.one,
    },

    // OK
    measurable: {
      type: graphql.GraphQLNonNull(measurablesTypes.measurable),
      resolve: resolvers.measurables.one,
    },

    // OK
    measurement: {
      type: measurementsTypes.measurement,
      args: {
        competitorType: {
          type: graphql.GraphQLList(measurementCompetitorType),
        },
      },
      resolve: resolvers.measurements.measurableMeasurement,
    },
  }),
});

const agentMeasurablesEdge = new graphql.GraphQLObjectType({
  name: 'AgentMeasurablesEdge',
  fields: () => ({
    node: { type: agentMeasurable },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const agentMeasurablesConnection = new graphql.GraphQLObjectType({
  name: 'AgentMeasurablesConnection',
  fields: () => ({
    total: { type: graphql.GraphQLInt },
    pageInfo: {
      type: graphql.GraphQLNonNull(commonTypes.pageInfoConnection),
    },
    edges: { type: graphql.GraphQLList(agentMeasurablesEdge) },
  }),
});

const orderFieldAgentMeasurables = new graphql.GraphQLEnumType({
  name: 'OrderFieldAgentMeasurables',
  values: {
    primaryPointScore: { value: 'primaryPointScore' },
  },
});

const orderAgentMeasurables = new graphql.GraphQLInputObjectType({
  name: 'OrderAgentMeasurables',
  fields: () => ({
    field: { type: graphql.GraphQLNonNull(orderFieldAgentMeasurables) },
    direction: {
      type: graphql.GraphQLNonNull(commonTypes.orderDirection),
    },
  }),
});

module.exports = {
  agentMeasurable,
  agentMeasurablesEdge,
  agentMeasurablesConnection,
  orderAgentMeasurables,
};
