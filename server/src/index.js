import express from 'express';
import path from 'path';
import * as auth0 from "auth0-js";
import logger from 'morgan';
import bodyParser from 'body-parser';

const models = require("./models")
const Sequelize = require('sequelize')

const jwt = require('jsonwebtoken')
var jwks = require('jwks-rsa')
const { ApolloServer, gql } = require('apollo-server-express');

import {schema} from './schema';
import { makeExecutableSchema } from 'graphql-tools';

const PORT = process.env.PORT || 4000;

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

function getToken(req) {
  if (req.headers.authorization && req.headers.authorization.split(' ')[0] === 'Bearer') {
      return req.headers.authorization.split(' ')[1];
  } else if (req.query && req.query.token) {
    return req.query.token;
  }
  return null;
}

const server = new ApolloServer({
  schema,
  formatError: error => {
    console.log("Error!", error);
    return error;
  },
  formatResponse: response => {
    return response;
  },
  context: async ({req}) => {
    const token = getToken(req);
    console.log("GOT TOKEN", token)
    const user = new Promise(resolve =>
      jwt.verify(token, process.env.AUTH0_SECRET, (err, result) => {
        console.log("HI", process.env.AUTH0_SECRET, err, result);
        console.log("Verify?", err, result)
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

    return {user}
  }
});


const app = express();
var cors = require("cors");
app.use(cors());
server.applyMiddleware({ app });

models.sequelize.sync().then(() => {
  app.listen({ port: PORT }, () =>{
    console.log(`🚀 Server ready at http://localhost:${PORT}${server.graphqlPath}`)
  });
})