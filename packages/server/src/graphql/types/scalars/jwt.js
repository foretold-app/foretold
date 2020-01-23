const graphql = require('graphql');

// eslint-disable-next-line no-useless-escape
const JWT_PATTERN = '^[A-Za-z0-9-_=]+\.[A-Za-z0-9-_=]+\.?[A-Za-z0-9-_.+/=]*$';

module.exports = new graphql.GraphQLScalarType({
  name: 'JWT',
  description: 'A special custom Scalar type for JWT',

  serialize(d) {
    return d;
  },

  parseValue(d) {
    return d;
  },

  parseLiteral(ast) {
    if (typeof ast.value !== 'string') {
      throw new Error('JWT should be a string.');
    }

    if (!(new RegExp(JWT_PATTERN).test(ast.value))) {
      throw new Error('JWT is invalid.');
    }

    return ast.value;
  },
});
