const graphql = require('graphql');
const { UserInputError } = require('apollo-server');

const PATTERN = '^[0-9a-f]{8}-[0-9a-f]{4}-[0-5][0-9a-f]{3}-[089ab][0-9a-f]{3}-[0-9a-f]{12}$';

module.exports = new graphql.GraphQLScalarType({
  name: 'ObjectID',
  description: 'A special custom scalar type for Object ID',

  serialize: function serialize(d) {
    return d;
  },

  parseValue: function parseValue(d) {
    return d;
  },

  parseLiteral: function parseLiteral(ast) {
    if (typeof ast.value !== 'string') {
      throw new UserInputError('ObjectID should be a string.');
    }

    if (!(new RegExp(PATTERN).test(ast.value))) {
      throw new UserInputError('ObjectID is invalid.');
    }

    return ast.value;
  },
});
