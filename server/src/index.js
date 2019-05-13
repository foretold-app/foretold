const _ = require('lodash');
const path = require('path');
const cors = require('cors');
const express = require('express');
const bodyParser = require('body-parser-graphql');
const { ApolloServer } = require('apollo-server-express');

const { schemaWithMiddlewares } = require('./schema');
const { authentication } = require('./authentication');
const { runJobs } = require('./async');
const { runListeners } = require('./async/listeners');

const PORT = process.env.PORT || 4000;

const server = new ApolloServer({
  introspection: true,
  playground: true,
  schema: schemaWithMiddlewares,
  formatError: error => {
    console.log("Error!", error);
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

const app = express();
app.use(cors());


const fallbackFile = path.resolve(__dirname, '../../client/dist/index.html');
const distDir = path.resolve(__dirname, '../../client/dist');

console.log('Fallback file', fallbackFile);
console.log('Dist dir', distDir);

// Returns all routes excluding "/graphql" as static files
// or returns fallback page.
app.get(/^((?!graphql).)*$/,
  express.static(distDir),
  (req, res) => res.sendFile(fallbackFile),
);


app.use(bodyParser.graphql());
server.applyMiddleware({ app });

app.listen({ port: PORT }, () => {
  console.log(`Server ready at http://localhost:${PORT}${server.graphqlPath}`)
});

runJobs();
runListeners();
