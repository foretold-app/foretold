const graphql = require('graphql');
const { UserInputError } = require('apollo-server');

function createLimitedString(min, max) {
  const name = `String${min}to${max}`;
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

      if (min !== undefined && ast.value.length < min) {
        throw new UserInputError(
          `Length of ${name} is less then required ${min}.`,
        );
      }

      if (max !== undefined && ast.value.length > max) {
        throw new UserInputError(
          `Length of ${name} is more then required ${max}.`,
        );
      }
      return ast.value;
    },
  });
}

module.exports = {
  createLimitedString,
};
