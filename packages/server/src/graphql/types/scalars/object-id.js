const graphql = require('graphql');
const { UserInputError } = require('apollo-server');

// eslint-disable-next-line max-len
const PATTERN = '^[a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}$';

function validate(name, value) {
  if (typeof value !== 'string') {
    throw new UserInputError(`${name} should be a string.`);
  }

  if (!(new RegExp(PATTERN).test(value))) {
    throw new UserInputError(`${name} is invalid.`);
  }

  return true;
}

function createObjectId(name) {
  return new graphql.GraphQLScalarType({
    name,
    description: `A special custom scalar type for a ${name}`,

    serialize(d) {
      return d;
    },

    parseValue(d) {
      validate(name, d);
      return d;
    },

    parseLiteral(ast) {
      validate(name, ast.value);
      return ast.value;
    },
  });
}


module.exports = {
  createObjectId,
};
