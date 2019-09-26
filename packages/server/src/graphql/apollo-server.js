const _ = require('lodash');
const { ApolloServer } = require('apollo-server-express');

const { schemaWithMiddlewares } = require('./schema');
const { initContext } = require('./middlewares/init-context');

const apolloServer = new ApolloServer({
  introspection: true,
  playground: true,
  schema: schemaWithMiddlewares,
  formatError: (error) => {
    console.error('Error!', error);
    console.error(error.extensions.exception.stacktrace);
    return error;
  },
  context: initContext,
});

module.exports = {
  apolloServer,
};
