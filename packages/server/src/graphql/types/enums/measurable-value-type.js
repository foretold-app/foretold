const graphql = require('graphql');

const { MEASURABLE_VALUE_TYPE } = require('../../../enums/measurable-value-type');

const measurableValueType = graphql.GraphQLNonNull(new graphql.GraphQLEnumType({
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
  measurableValueType,
};
