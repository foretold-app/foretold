const graphql = require('graphql');

const { MEASURABLE_VALUE_TYPE } = require('../models/enums/measurable-value-type');

const valueType = graphql.GraphQLNonNull(new graphql.GraphQLEnumType({
  name: 'valueType',
  values: {
    [MEASURABLE_VALUE_TYPE.FLOAT]: {
      value: MEASURABLE_VALUE_TYPE.FLOAT,
    },
    [MEASURABLE_VALUE_TYPE.DATE]: {
      value: MEASURABLE_VALUE_TYPE.DATE,
    },
    [MEASURABLE_VALUE_TYPE.PERCENTAGE]: {
      value: MEASURABLE_VALUE_TYPE.PERCENTAGE,
    },
  }
}));

module.exports = {
  valueType,
};
