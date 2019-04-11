const graphql = require("graphql");

const permission = new graphql.GraphQLObjectType({
  name: 'Permission',
  fields: () => ({
    allow: { type: graphql.GraphQLList(graphql.GraphQLString) },
    deny: { type: graphql.GraphQLList(graphql.GraphQLString) },
  }),
});

const permissions = new graphql.GraphQLObjectType({
  name: 'Permissions',
  fields: () => ({
    mutations: { type: permission },
    queries: { type: permission },
  }),
});

module.exports = {
  permission,
  permissions,
};
