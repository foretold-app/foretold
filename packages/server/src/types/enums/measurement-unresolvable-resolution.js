const graphql = require('graphql');

const { UNRESOLVABLE_RESOLUTIONS } = require('../../models/enums/unresolvable-resolutions');

const measurementUnresolvableResolution = new graphql.GraphQLEnumType({
  name: 'unresolvableResolution',
  values: {
    [UNRESOLVABLE_RESOLUTIONS.AMBIGUOUS]: {
      value: UNRESOLVABLE_RESOLUTIONS.AMBIGUOUS,
    },
    [UNRESOLVABLE_RESOLUTIONS.RESULT_NOT_AVAILABLE]: {
      value: UNRESOLVABLE_RESOLUTIONS.RESULT_NOT_AVAILABLE,
    },
    [UNRESOLVABLE_RESOLUTIONS.FALSE_CONDITIONAL]: {
      value: UNRESOLVABLE_RESOLUTIONS.FALSE_CONDITIONAL,
    },
    [UNRESOLVABLE_RESOLUTIONS.OTHER]: {
      value: UNRESOLVABLE_RESOLUTIONS.OTHER,
    },
  }
});

module.exports = {
  measurementUnresolvableResolution,
};
