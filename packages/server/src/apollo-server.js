const { ApolloServer } = require('apollo-server-express');

const { schemaWithMiddlewares } = require('./schema');
const { authentication } = require('./authentication');

const apolloServer = new ApolloServer({
  introspection: true,
  playground: true,
  schema: schemaWithMiddlewares,
  formatError: error => {
    console.error("Error!", error);
    console.error(error.extensions.exception.stacktrace);
    return error;
  },
  formatResponse: response => {
    return response;
  },
  context: async ({ req }) => {
    const context = await authentication(req);
    console.log(' --- ');
    console.log(' ✓ Context User Id', _.get(context, 'user.id'));
    console.log(' ✓ Context Agent Id', _.get(context, 'agent.id'));
    console.log(' ✓ Context Bot Id', _.get(context, 'bot.id'));
    console.log(' ✓ Context Creator Id', _.get(context, 'creator.id'));
    console.log(' ✓ Context Creator Name', _.get(context, 'creator.constructor.name'));
    console.log(' --- ');
    return context;
  }
});

module.exports = {
  apolloServer,
};
