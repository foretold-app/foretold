const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const { mutexStatus } = require('./enums/mutex-status');

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
    status: { type: graphql.GraphQLNonNull(mutexStatus) },
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
  }),
});

module.exports = {
  mutex,
  mutexCreateInput,
};
