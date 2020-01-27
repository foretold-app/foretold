const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const scalars = require('./scalars');

const vote = new graphql.GraphQLObjectType({
  name: 'Vote',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(scalars.voteId) },
    agentId: { type: graphql.GraphQLNonNull(scalars.agentId) },
    measurementId: { type: graphql.GraphQLNonNull(scalars.measurementId) },
    voteAmount: { type: graphql.GraphQLNonNull(graphql.GraphQLInt) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
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
