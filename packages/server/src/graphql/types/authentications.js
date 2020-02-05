const graphql = require('graphql');

const usersTypes = require('./users');
const botsTypes = require('./bots');
const agentsTypes = require('./agents');

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
    user: { type: usersTypes.user },
    bot: { type: botsTypes.bot },
    agent: { type: agentsTypes.agent },
  }),
});

module.exports = {
  authenticated,
  authenticationJwt,
  authenticationToken,
};
