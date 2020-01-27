const graphql = require('graphql');
const { UserInputError } = require('apollo-server');

/**
 * @param value
 * @returns {boolean}
 */
function validate(value) {
  if (!Number.isInteger(value)) {
    throw new UserInputError('Vote is not an integer.');
  }
  if (value > 10) {
    throw new UserInputError('Vote is more than 10.');
  }
  if (value < -10) {
    throw new UserInputError('Vote is less than 10.');
  }
  return true;
}

const voteAmount = new graphql.GraphQLScalarType({
  name: 'VoteAmount',
  description: 'A special custom scalar type for a vote amount',

  serialize(d) {
    return d;
  },

  parseValue(d) {
    validate(d);
    return d;
  },

  parseLiteral(ast) {
    validate(ast.value);
    return ast.value;
  },
});

module.exports = {
  voteAmount,
};
