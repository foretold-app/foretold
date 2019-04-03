const graphql = require("graphql");

const { MEASURABLE_STATE } = require('../models/measurable-state');

const measurableState = graphql.GraphQLNonNull(new graphql.GraphQLEnumType({
  name: 'measurableState',
  values: {
    [MEASURABLE_STATE.OPEN]: { value: MEASURABLE_STATE.OPEN },
    [MEASURABLE_STATE.ARCHIVED]: { value: MEASURABLE_STATE.ARCHIVED },
    [MEASURABLE_STATE.JUDGEMENT_PENDING]: { value: MEASURABLE_STATE.JUDGEMENT_PENDING },
    [MEASURABLE_STATE.JUDGED]: { value: MEASURABLE_STATE.JUDGED },
  }
}));

module.exports = {
  measurableState,
};
