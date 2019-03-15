const path = require('path');
import express from 'express';
import { schema } from './schema';

const jwt = require('jsonwebtoken');
const { ApolloServer } = require('apollo-server-express');

const models = require("./models");

const PORT = process.env.PORT || 4000;

function getToken(req) {
  if (req.headers.authorization && req.headers.authorization.split(' ')[0] === 'Bearer') {
    return req.headers.authorization.split(' ')[1];
  } else if (req.query && req.query.token) {
    return req.query.token;
  }
  return null;
}

const server = new ApolloServer({
  introspection: true,
  playground: true,
  schema,
  formatError: error => {
    console.log("Error!", error);
    return error;
  },
  formatResponse: response => {
    return response;
  },
  context: async ({ req }) => {
    const token = getToken(req);
    const user = new Promise(resolve =>
      jwt.verify(token, process.env.AUTH0_SECRET,(err, result) => {
        if (err) {
          resolve({
            ok: false,
            result: err
          });
        } else {
          resolve({
            ok: true,
            result
          });
        }
      })
    );
    return { user };
  }
});

const app = express();
const cors = require("cors");
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

server.applyMiddleware({ app });

models.sequelize.sync().then(() => {
  app.listen({ port: PORT }, () => {
    console.log(`🚀 Server ready at http://localhost:${PORT}${server.graphqlPath}`)
  });
})
