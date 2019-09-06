const _ = require('lodash');
const { ApolloServer } = require('apollo-server-express');

const { schemaWithMiddlewares } = require('./schema');
const { authentication } = require('./authentication');

const { globalSettings } = require('./data');

const apolloServer = new ApolloServer({
  introspection: true,
  playground: true,
  schema: schemaWithMiddlewares,
  formatError: (error) => {
    console.error("Error!", error);
    console.error(error.extensions.exception.stacktrace);
    return error;
  },
  context: async ({ req }) => {
    const identity = await authentication(req);
    const botAgentId = await globalSettings.getBotAgentId();

    const initContext = { ...identity, botAgentId };

    console.log(' --- ');
    console.log(' ✓ Context Identity User Id', _.get(initContext, 'user.id'));
    console.log(' ✓ Context Identity Agent Id', _.get(initContext, 'agent.id'));
    console.log(' ✓ Context Identity Bot Id', _.get(initContext, 'bot.id'));
    console.log(' ✓ Context Identity Creator Id', _.get(initContext, 'creator.id'));
    console.log(' ✓ Context Identity Creator Name', _.get(initContext, 'creator.constructor.name'));
    console.log(' ✓ Context Settings Bot Agent Id', _.get(initContext, 'botAgentId'));
    console.log(' --- ');

    return initContext;
  }
});

module.exports = {
  apolloServer,
};
