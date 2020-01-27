const graphql = require('graphql');

const { INVITATION_STATUS } = require('../../../enums');

const invitationStatus = new graphql.GraphQLEnumType({
  name: 'invitationStatus',
  values: {
    [INVITATION_STATUS.AWAITING]: {
      value: INVITATION_STATUS.AWAITING,
    },
    [INVITATION_STATUS.ACCEPTED]: {
      value: INVITATION_STATUS.ACCEPTED,
    },
  },
});

module.exports = {
  invitationStatus,
};
