const graphql = require('graphql');

const { queries } = require('./enums');
const { mutations } = require('./enums');

const permissionQueries = new graphql.GraphQLObjectType({
  name: 'PermissionQueries',
  fields: () => ({
    allow: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(queries),
      ),
    },
    deny: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(queries),
      ),
    },
  }),
});

const permissionMutations = new graphql.GraphQLObjectType({
  name: 'PermissionMutations',
  fields: () => ({
    allow: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(mutations),
      ),
    },
    deny: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(mutations),
      ),
    },
  }),
});

const permissions = new graphql.GraphQLObjectType({
  name: 'Permissions',
  fields: () => ({
    mutations: { type: graphql.GraphQLNonNull(permissionMutations) },
    queries: { type: graphql.GraphQLNonNull(permissionQueries) },
  }),
});

module.exports = {
  permissions,
};
