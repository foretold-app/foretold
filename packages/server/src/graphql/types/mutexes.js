const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const mutexCreateInput = new graphql.GraphQLInputObjectType({
  name: 'MutexCreateInput',
  fields: () => ({
    name: { type: graphql.GraphQLString },
  }),
});

const mutex = new graphql.GraphQLObjectType({
  name: 'Mutex',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
  }),
});

module.exports = {
  mutex,
  mutexCreateInput,
};
