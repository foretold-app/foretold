const graphql = require("graphql");

const authenticationJwt = new graphql.GraphQLObjectType({
  name: 'AuthenticationJwt',
  fields: () => ({
    jwt: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const authenticationToken = new graphql.GraphQLObjectType({
  name: 'AuthenticationToken',
  fields: () => ({
    token: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

module.exports = {
  authenticationJwt,
  authenticationToken,
};
