const graphql = require('graphql');

const {
  MARKET_TYPE,
  START_AT,
  FINAL_COMPARISON_MEASUREMENT,
} = require('../../../enums');

// @todo: To rename the file.

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
    [START_AT.QUESTION_CREATION_TIME]: {
      value: START_AT.QUESTION_CREATION_TIME,
    },
    [START_AT.AGENT_MEASUREMENT_CREATION_TIME]: {
      value: START_AT.AGENT_MEASUREMENT_CREATION_TIME,
    },
  },
}));

const finalComparisonMeasurement = graphql.GraphQLNonNull(
  new graphql.GraphQLEnumType({
    name: 'finalComparisonMeasurement',
    values: {
      [FINAL_COMPARISON_MEASUREMENT.LAST_OBJECTIVE_MEASUREMENT]: {
        value: FINAL_COMPARISON_MEASUREMENT.LAST_OBJECTIVE_MEASUREMENT,
      },
      [FINAL_COMPARISON_MEASUREMENT.LAST_AGGREGATE_MEASUREMENT]: {
        value: FINAL_COMPARISON_MEASUREMENT.LAST_AGGREGATE_MEASUREMENT,
      },
    },
  }),
);

module.exports = {
  marketScoreType,
  startAt,
  finalComparisonMeasurement,
};
