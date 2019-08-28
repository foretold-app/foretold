const graphql = require("graphql");
const { DateType } = require('graphql-sequelize');

const agentMeasurable = new graphql.GraphQLObjectType({
  name: 'AgentMeasurable',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    measurableId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    primaryPointScore: {
      type: graphql.GraphQLNonNull(graphql.GraphQLFloat),
      resolve: () => 0.7
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

    agentMeasurement: {
      type: require('./measurements').measurement,
      args: {
        competitorType: {
          type: graphql.GraphQLList(
            require('./enums/measurement-competitor-type')
              .measurementCompetitorType,
          ),
        },
        isLinkedWithAgent: { type: graphql.GraphQLBoolean },
      },
      resolve: require('../resolvers').measurements.firstA,
    },

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
      resolve: require('../resolvers').measurements.firstB,
    },
  })
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
    pageInfo: { type: graphql.GraphQLNonNull(require('./common').pageInfoConnection) },
    edges: { type: graphql.GraphQLList(require('./agent-measurables').agentMeasurablesEdge) },
  }),
});

module.exports = {
  agentMeasurable,
  agentMeasurablesEdge,
  agentMeasurablesConnection,
};
