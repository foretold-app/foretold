const graphql = require("graphql");

const { MEASURABLE_STATE } = require('../models/measurable-state');

const measurableState = graphql.GraphQLNonNull(graphql.GraphQLEnumType({
  name: 'measurableState',
  values: {
    [MEASURABLE_STATE.OPEN]: { value: MEASURABLE_STATE.OPEN },
    [MEASURABLE_STATE.ARCHIVED]: { value: MEASURABLE_STATE.ARCHIVED },
    [MEASURABLE_STATE.JUDGMENT_PENDING]: { value: MEASURABLE_STATE.JUDGMENT_PENDING },
    [MEASURABLE_STATE.JUDGED]: { value: MEASURABLE_STATE.JUDGED },
  }
}));

module.exports = {
  measurableState,
};
