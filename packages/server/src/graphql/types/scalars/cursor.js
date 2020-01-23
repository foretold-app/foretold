const graphql = require('graphql');
const { UserInputError } = require('apollo-server');

const PATTERN = '^[0-9]{9}$';

module.exports = new graphql.GraphQLScalarType({
  name: 'Cursor',
  description: 'A special custom scalar type for a Cursor',

  serialize(d) {
    return d;
  },

  parseValue(d) {
    return d;
  },

  parseLiteral(ast) {
    if (typeof ast.value !== 'string') {
      throw new UserInputError('Cursor should be a string.');
    }

    if (!(new RegExp(PATTERN).test(ast.value))) {
      throw new UserInputError('Cursor is invalid.');
    }

    return ast.value;
  },
});
