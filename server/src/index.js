import express from 'express';
import path from 'path';
import * as auth0 from "auth0-js";
import logger from 'morgan';
import bodyParser from 'body-parser';

const models = require("./models")
const Sequelize = require('sequelize')

const jwt = require('express-jwt')
var jwks = require('jwks-rsa')
const { ApolloServer, gql } = require('apollo-server-express');

import {schema} from './schema';
import { makeExecutableSchema } from 'graphql-tools';

const typeDefs = gql`
  type Query {
    hello: String
  }
`;
 
// Provide resolver functions for your schema fields
const resolvers = {
  Query: {
    hello: () => 'Hello world!',
  },
};

const server = new ApolloServer({
  schema,
  formatError: error => {
    return error;
  },
  formatResponse: response => {
    return response;
  },
  context: ({req}) => {
    return {userSub: req.user && req.user.sub}
}
});


function getToken(req) {
  if (req.headers.authorization && req.headers.authorization.split(' ')[0] === 'Bearer') {
      return req.headers.authorization.split(' ')[1];
  } else if (req.query && req.query.token) {
    return req.query.token;
  }
  return null;
}
const app = express();
var cors = require("cors");
app.use(cors());
app.use(jwt({secret: "bhz9XiFVqoowf_cSicdItfmExxWrAoeyhKEjGNQKjpX08E0NKuLNQ3uF5XL-wdy_", credentialsRequired: false, getToken}))
server.applyMiddleware({ app });

app.listen({ port: 4000 }, () =>
  console.log(`🚀 Server ready at http://localhost:4000${server.graphqlPath}`)
);