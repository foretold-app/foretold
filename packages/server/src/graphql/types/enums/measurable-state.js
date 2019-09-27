const graphql = require('graphql');

const { MEASURABLE_STATE } = require('../../../enums/measurable-state');

const measurableState = new graphql.GraphQLEnumType({
  name: 'measurableState',
  values: {
    [MEASURABLE_STATE.OPEN]: {
      value: MEASURABLE_STATE.OPEN,
    },
    [MEASURABLE_STATE.JUDGED]: {
      value: MEASURABLE_STATE.JUDGED,
    },
    [MEASURABLE_STATE.JUDGEMENT_PENDING]: {
      value: MEASURABLE_STATE.JUDGEMENT_PENDING,
    },
    [MEASURABLE_STATE.CLOSED_AS_UNRESOLVED]: {
      value: MEASURABLE_STATE.CLOSED_AS_UNRESOLVED,
    },
  },
});

module.exports = {
  measurableState,
};
