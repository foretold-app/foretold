const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');
const resolvers = require('../resolvers');

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
    points: {
      type: points,
    },
  }),
});

const timeAverageScore = new graphql.GraphQLObjectType({
  name: 'timeAverageScore',
  fields: () => ({
    score: { type: graphql.GraphQLFloat },
    agentPredictions: {
      type: graphql.GraphQLList(require('./measurements').measurement),
    },
    aggregations: {
      type: graphql.GraphQLList(require('./measurements').measurement),
    },
    recentResult: {
      type: require('./measurements').measurement,
    },
    scoringStartTime: { type: DateType.default },
    scoringEndTime: { type: DateType.default },
    measurableCreationTime: { type: DateType.default },
    finalResolutionTime: { type: DateType.default },
    timeActivityRatio: { type: graphql.GraphQLFloat },
    activeTimeDistribution: { type: activeTimeDistribution },
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
        marketType: {
          type: require('./enums/agent-measurable-score-params')
            .marketScoreType,
        },
        startAt: {
          type: require('./enums/agent-measurable-score-params').startAt,
        },
        finalComparisonMeasurement: {
          type: require('./enums/agent-measurable-score-params')
            .finalComparisonMeasurement,
        },
      },
    },
    predictionCountTotal: { type: graphql.GraphQLNonNull(graphql.GraphQLInt) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },

    // OK
    agent: {
      type: graphql.GraphQLNonNull(require('./agents').agent),
      resolve: resolvers.agents.one,
    },

    // OK
    measurable: {
      type: graphql.GraphQLNonNull(require('./measurables').measurable),
      resolve: resolvers.measurables.one,
    },

    // OK
    measurement: {
      type: require('./measurements').measurement,
      args: {
        competitorType: {
          type: graphql.GraphQLList(
            require('./enums/measurement-competitor-type')
              .measurementCompetitorType,
          ),
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
      type: graphql.GraphQLNonNull(require('./common').pageInfoConnection),
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
      type: graphql.GraphQLNonNull(
        require('./common').orderDirection,
      ),
    },
  }),
});

module.exports = {
  agentMeasurable,
  agentMeasurablesEdge,
  agentMeasurablesConnection,
  orderAgentMeasurables,
};
