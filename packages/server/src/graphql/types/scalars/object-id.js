const graphql = require('graphql');
const { UserInputError } = require('apollo-server');

// eslint-disable-next-line max-len
const PATTERN = '^/[0-9a-fA-F]{8}\\-[0-9a-fA-F]{4}\\-[0-9a-fA-F]{4}\\-[0-9a-fA-F]{4}\\-[0-9a-fA-F]{12}/$';

function createObjectId(name) {
  return new graphql.GraphQLScalarType({
    name: name,
    description: `A special custom scalar type for a ${name}`,

    serialize: function serialize(d) {
      return d;
    },

    parseValue: function parseValue(d) {
      return d;
    },

    parseLiteral: function parseLiteral(ast) {
      if (typeof ast.value !== 'string') {
        throw new UserInputError(`${name} should be a string.`);
      }

      if (!(new RegExp(PATTERN).test(ast.value))) {
        throw new UserInputError(`${name} is invalid.`);
      }

      return ast.value;
    },
  });
}


module.exports = {
  createObjectId,
};
