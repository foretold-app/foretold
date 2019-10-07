const { GraphQLScalarType } = require('graphql');
const { UserInputError } = require('apollo-server');

class LimitedLengthType extends GraphQLScalarType {
  constructor(type, maxLength) {
    super({
      name: `String${maxLength}`,

      serialize(value) {
        value = type.serialize(value);
        return value;
      },

      parseValue(value) {
        if (value.length > maxLength) {
          throw new UserInputError(
            `Length is limited to "${maxLength}" characters.`,
          );
        }
        return type.parseValue(value);
      },

      parseLiteral(ast) {
        return type.parseLiteral(ast);
      }
    });
  }
}

module.exports = {
  LimitedLengthType,
};
