const _ = require('lodash');
const { ApolloServer } = require('apollo-server-express');
const depthLimit = require('graphql-depth-limit');

const { schemaWithMiddlewares } = require('./schema');
const { context } = require('./middlewares/context');
const logger = require('../lib/log');

const log = logger.module('graphql/apollo-server');

class ApolloServerDepth extends ApolloServer {
  async createGraphQLServerOptions(req, res) {
    const options = await super.createGraphQLServerOptions(req, res);
    return {
      ...options,
      validationRules: [
        depthLimit(10),
      ],
    };
  }
}

function getApolloServer() {
  return new ApolloServerDepth({
    introspection: true,
    playground: true,
    schema: schemaWithMiddlewares,
    formatError: (error) => {
      log.error('Error!', error);
      log.error(_.get(error, 'extensions.exception.stacktrace'));
      return error;
    },
    context,
  });
}

module.exports = {
  getApolloServer,
};
