const graphql = require("graphql");

const { MEASUREMENT_COMPETITOR_TYPE } = require('../models/measurement-competitor-type');

const competitorType = graphql.GraphQLNonNull(new graphql.GraphQLEnumType({
  name: 'competitorType',
  values: {
    [MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE]: {
      value: MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE,
    },
    [MEASUREMENT_COMPETITOR_TYPE.AGGREGATION]: {
      value: MEASUREMENT_COMPETITOR_TYPE.AGGREGATION,
    },
    [MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE]: {
      value: MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE,
    },
  }
}));

module.exports = {
  competitorType,
};
