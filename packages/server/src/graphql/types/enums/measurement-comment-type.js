const graphql = require('graphql');

const { MEASUREMENT_COMMENT_TYPE } = require('../../../enums');

const measurementCommentType = new graphql.GraphQLEnumType({
  name: 'commentType',
  values: {
    [MEASUREMENT_COMMENT_TYPE.GENERIC]: {
      value: MEASUREMENT_COMMENT_TYPE.GENERIC,
    },
    [MEASUREMENT_COMMENT_TYPE.QUESTION_FEEDBACK]: {
      value: MEASUREMENT_COMMENT_TYPE.QUESTION_FEEDBACK,
    },
    [MEASUREMENT_COMMENT_TYPE.UPDATE]: {
      value: MEASUREMENT_COMMENT_TYPE.UPDATE,
    },
  },
});

module.exports = {
  measurementCommentType,
};
