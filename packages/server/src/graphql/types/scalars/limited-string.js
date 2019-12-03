const graphql = require('graphql');
const { UserInputError } = require('apollo-server');

/**
 * @param {string} name
 * @param {number} min
 * @param {number} max
 * @returns {function(*=): string}
 */
function validateFactory(name, min, max) {
  /**
   * @params {string} value
   */
  return function validate(value) {
    if (typeof value !== 'string') {
      throw new UserInputError(`${name} should be a string.`);
    }

    if (min !== undefined && value.length < min) {
      throw new UserInputError(
        `Length of ${name} is less then required ${min}.`,
      );
    }

    if (max !== undefined && value.length > max) {
      throw new UserInputError(
        `Length of ${name} is more then required ${max}.`,
      );
    }

    return value;
  };
}

/**
 * @param {number} min
 * @param {number} max
 * @returns {GraphQLScalarType}
 */
function createLimitedString(min, max) {
  const name = `String${min}to${max}`;
  const validate = validateFactory(name, min, max);
  return new graphql.GraphQLScalarType({
    name,
    description: `A special custom scalar type for a ${name}`,

    serialize: function serialize(value) {
      return validate(value);
    },

    parseValue: function parseValue(value) {
      return validate(value);
    },

    parseLiteral: function parseLiteral(ast) {
      return ast.value;
    },
  });
}

module.exports = {
  createLimitedString,
};
