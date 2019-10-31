const graphql = require('graphql');

const {
  MARKET_TYPE,
  START_AT,
  FINAL_SCORE_TYPE,
} = require('../../../enums/agent-measurable-score-type');

const marketScoreType = graphql.GraphQLNonNull(new graphql.GraphQLEnumType({
  name: 'marketScoreType',
  values: {
    [MARKET_TYPE.MARKET]: {
      value: MARKET_TYPE.MARKET,
    },
    [MARKET_TYPE.NON_MARKET]: {
      value: MARKET_TYPE.NON_MARKET,
    },
  },
}));

const startAt = graphql.GraphQLNonNull(new graphql.GraphQLEnumType({
  name: 'startAt',
  values: {
    [START_AT.QUESTION_START]: {
      value: START_AT.QUESTION_START,
    },
    [START_AT.AGENT_MEASUREMENT_START]: {
      value: START_AT.AGENT_MEASUREMENT_START,
    },
  },
}));

const finalScoreType = graphql.GraphQLNonNull(new graphql.GraphQLEnumType({
  name: 'finalScoreType',
  values: {
    [FINAL_SCORE_TYPE.LAST_OBJECTIVE_MEASUREMENT]: {
      value: FINAL_SCORE_TYPE.LAST_OBJECTIVE_MEASUREMENT,
    },
    [FINAL_SCORE_TYPE.LAST_AGGREGATE_MEASUREMENT]: {
      value: FINAL_SCORE_TYPE.LAST_AGGREGATE_MEASUREMENT,
    },
  },
}));

module.exports = {
  marketScoreType,
  startAt,
  finalScoreType,
};
