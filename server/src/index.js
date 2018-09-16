import express from 'express';
import path from 'path';
import logger from 'morgan';
import bodyParser from 'body-parser';
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

const server = new ApolloServer({schema,   formatError: error => {
  console.log(error);
  return error;
},
formatResponse: response => {
  console.log(response);
  return response;
},});

const app = express();
var cors = require("cors");
app.use(cors());
server.applyMiddleware({ app });

app.listen({ port: 4000 }, () =>
  console.log(`🚀 Server ready at http://localhost:4000${server.graphqlPath}`)
);