const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const agentMeasurable = new graphql.GraphQLObjectType({
  name: 'AgentMeasurable',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    measurableId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    primaryPointScore: {
      type: graphql.GraphQLFloat,
      resolve: require('../resolvers').agentMeasurables.primaryPointScore,
    },
    timeAverageScore: {
      type: new graphql.GraphQLObjectType({
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
          activeTimeDistribution: {
            type: new graphql.GraphQLObjectType({
              name: 'timeDistribution',
              fields: () => ({
                finalX: { type: graphql.GraphQLInt },
                points: {
                  type: graphql.GraphQLList(new graphql.GraphQLObjectType({
                    name: 'point',
                    fields: () => ({
                      x: { type: graphql.GraphQLInt },
                      y: { type: graphql.GraphQLFloat },
                    }),
                  })),
                },
              }),
            }),
          },
        }),
      }),
      resolve: require('../resolvers').agentMeasurables.timeAverageScore,
      args: {
        marketType: {
          type: require('./enums/agent-measurable-score-params').marketScoreType
        },
        startAt: {
          type: require('./enums/agent-measurable-score-params').startAt
        },
        finalComparisonMeasurement: {
          type: require('./enums/agent-measurable-score-params')
            .finalComparisonMeasurement
        },
      },
    },
    predictionCountTotal: { type: graphql.GraphQLNonNull(graphql.GraphQLInt) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },

    // OK
    agent: {
      type: graphql.GraphQLNonNull(require('./agents').agent),
      resolve: require('../resolvers').agents.one,
    },

    // OK
    measurable: {
      type: graphql.GraphQLNonNull(require('./measurables').measurable),
      resolve: require('../resolvers').measurables.one,
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
      resolve: require('../resolvers').measurements.measurableMeasurement,
    },
  }),
});

const agentMeasurablesEdge = new graphql.GraphQLObjectType({
  name: 'AgentMeasurablesEdge',
  fields: () => ({
    node: { type: require('./agent-measurables').agentMeasurable },
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
    edges: {
      type: graphql.GraphQLList(
        require('./agent-measurables').agentMeasurablesEdge,
      ),
    },
  }),
});

module.exports = {
  agentMeasurable,
  agentMeasurablesEdge,
  agentMeasurablesConnection,
};
