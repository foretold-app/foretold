const graphql = require("graphql");

const authentication = new graphql.GraphQLObjectType({
  name: 'Authentication',
  fields: () => ({
    jwt: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

module.exports = {
  authentication,
};
