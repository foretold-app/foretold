const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const scalars = require('./scalars');
const agentTypes = require('./agents');
const resolvers = require('../resolvers');

const vote = new graphql.GraphQLObjectType({
  name: 'Vote',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(scalars.voteId) },
    agentId: { type: graphql.GraphQLNonNull(scalars.agentId) },
    measurementId: { type: graphql.GraphQLNonNull(scalars.measurementId) },
    voteAmount: { type: graphql.GraphQLNonNull(graphql.GraphQLInt) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },

    agent: {
      type: agentTypes.agent,
      resolve: resolvers.agents.one,
    },
  }),
});

const voteInput = new graphql.GraphQLInputObjectType({
  name: 'VoteInput',
  fields: () => ({
    voteAmount: { type: graphql.GraphQLNonNull(scalars.voteAmount) },
  }),
});

module.exports = {
  vote,
  voteInput,
};
