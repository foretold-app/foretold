const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const scalars = require('./scalars');

const mutexCreateInput = new graphql.GraphQLInputObjectType({
  name: 'MutexCreateInput',
  fields: () => ({
    name: { type: graphql.GraphQLString },
  }),
});

const mutex = new graphql.GraphQLObjectType({
  name: 'Mutex',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(scalars.$mutexId) },
    name: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    agentId: { type: graphql.GraphQLNonNull(scalars.$agentId) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
  }),
});

module.exports = {
  mutex,
  mutexCreateInput,
};
