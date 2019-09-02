const graphql = require('graphql');

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

const authenticated = new graphql.GraphQLObjectType({
  name: 'Authenticated',
  fields: () => ({
    user: { type: require('./users').user },
    bot: { type: require('./bots').bot },
    agent: { type: require('./agents').agent },
  }),
});

module.exports = {
  authenticated,
  authenticationJwt,
  authenticationToken,
};
