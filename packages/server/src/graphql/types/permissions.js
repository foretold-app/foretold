const graphql = require('graphql');

const permissionQueries = new graphql.GraphQLObjectType({
  name: 'PermissionQueries',
  fields: () => ({
    allow: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(require('./queries').queries),
      ),
    },
    deny: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(require('./queries').queries),
      ),
    },
  }),
});

const permissionMutations = new graphql.GraphQLObjectType({
  name: 'PermissionMutations',
  fields: () => ({
    allow: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(require('./mutations').mutations),
      ),
    },
    deny: {
      type: graphql.GraphQLNonNull(
        graphql.GraphQLList(require('./mutations').mutations),
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
