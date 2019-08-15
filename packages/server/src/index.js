const _ = require('lodash');
const path = require('path');
const cors = require('cors');
const express = require('express');
const bodyParser = require('body-parser-graphql');
const { ApolloServer } = require('apollo-server-express');

const config = require('./config');
const { schemaWithMiddlewares } = require('./schema');
const { authentication } = require('./authentication');
const { runJobs } = require('./async');
const { runListeners } = require('./async/listeners');
const SERVER_IS_READY = require('./async/events');
const emitter = require('./async/emitter');

{
  runJobs();
  runListeners();
}

const app = express();
app.use(cors());

{
  const fallbackFile = path.resolve(__dirname, '../../client/dist/index.html');
  const distDir = path.resolve(__dirname, '../../client/dist');

  console.log('Fallback file', fallbackFile);
  console.log('Dist dir', distDir);

  // Returns all routes excluding "/graphql" as static files
  // or returns fallback page.
  app.get(/^((?!(graphql|hooks)).)*$/,
    express.static(distDir),
    (req, res) => res.sendFile(fallbackFile),
  );
}

{
  const { app: subApp } = require('./github/app');
  app.use('/hooks', subApp);
}

{
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
  app.use(bodyParser.graphql());
  apolloServer.applyMiddleware({ app });
}

app.listen({ port: config.PORT }, () => {
  console.log(`Server ready at http://localhost:${config.PORT}`);
  emitter.emit(SERVER_IS_READY, app);
});
