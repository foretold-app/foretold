const { getPermissions } = require('./permissions');

const { availableAll } = require('./availability');
const { availableChannelMutations } = require('./availability');
const { availableChannelMembershipsMutations } = require('./availability');

const permissions = getPermissions();

module.exports = {
  availableAll,
  availableChannelMutations,
  availableChannelMembershipsMutations,
  permissions,
};
