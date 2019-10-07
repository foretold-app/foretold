const graphql = require('graphql');
const { UserInputError } = require('apollo-server');

const JWT_PATTERN = '^[A-Za-z0-9-_=]+\.[A-Za-z0-9-_=]+\.?[A-Za-z0-9-_.+/=]*$';

module.exports = new graphql.GraphQLScalarType({
  name: 'JWT',
  description: 'A special custom Scalar type for JWT',

  serialize: function serialize(d) {
    return d;
  },

  parseValue: function parseValue(d) {
    return d;
  },

  parseLiteral: function parseLiteral(ast) {
    if (typeof ast.value !== 'string') {
      throw new UserInputError('JWT should be a string.');
    }

    if (!(new RegExp(JWT_PATTERN).test(ast.value))) {
      throw new UserInputError('JWT is invalid.');
    }

    return ast.value;
  },
});
