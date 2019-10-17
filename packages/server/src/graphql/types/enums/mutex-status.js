const graphql = require('graphql');

const { MUTEX_STATUS } = require('../../../enums/mutex-status');

const mutexStatus = new graphql.GraphQLEnumType({
  name: 'mutexStatus',
  values: {
    [MUTEX_STATUS.FREE]: {
      value: MUTEX_STATUS.FREE,
    },
    [MUTEX_STATUS.CAPTURED]: {
      value: MUTEX_STATUS.CAPTURED,
    },
  },
});

module.exports = {
  mutexStatus,
};
