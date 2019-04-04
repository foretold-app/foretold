const graphql = require("graphql");

const { MEASURABLE_STATE } = require('../models/measurable-state');

const measurableState = graphql.GraphQLNonNull(new graphql.GraphQLEnumType({
  name: 'measurableState',
  values: {
    [MEASURABLE_STATE.OPEN]: { value: MEASURABLE_STATE.OPEN },
    [MEASURABLE_STATE.JUDGED]: { value: MEASURABLE_STATE.JUDGED },
    [MEASURABLE_STATE.JUDGEMENT_PENDING]: { value: MEASURABLE_STATE.JUDGEMENT_PENDING },
  }
}));

module.exports = {
  measurableState,
};
