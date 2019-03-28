const path = require('path');
const cors = require('cors');
const express = require('express');
const bodyParser = require('body-parser-graphql');
const { ApolloServer } = require('apollo-server-express');

const models = require("./models");
const { schemaWithMiddlewares } = require('./schema');
const { authentication } = require('./authentication');

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
  context: async ({req}) => {
    const user = await authentication(req);
    return { user };
  }
});

const app = express();
app.use(cors());

// Returns all routes excluding "/graphql" as static files
// or returns fallback page.
app.get(/^((?!graphql).)*$/,
  express.static('../client/dist'),
  // Fallback
  (req, res) => {
    res.sendFile(path.resolve(__dirname, '../../client/dist/index.html'));
  }
);

app.use(bodyParser.graphql());
server.applyMiddleware({ app });

models.sequelize.sync().then(() => {
  app.listen({ port: PORT }, () => {
    console.log(`🚀 Server ready at http://localhost:${PORT}${server.graphqlPath}`)
  });
});
