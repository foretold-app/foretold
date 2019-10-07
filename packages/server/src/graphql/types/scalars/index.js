const graphql = require('graphql');

const { LimitedLengthType } = require('./limited-length-type');
const jwt = require('./jwt');

const string64 = new LimitedLengthType(graphql.GraphQLString, 64);
const string512 = new LimitedLengthType(graphql.GraphQLString, 512);

module.exports = {
  jwt,
  string64,
  string512,
  LimitedLengthType,
};
