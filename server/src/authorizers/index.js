const { getPermissions } = require('./permissions');

const { availableAll } = require('./availability');
const { availableChannelPermissions } = require('./availability');
const { availableChannelMembershipsPermissions } = require('./availability');
const { availableMeasurablesPermissions } = require('./availability');

const permissions = getPermissions();

module.exports = {
  availableAll,
  availableChannelPermissions,
  availableMeasurablesPermissions,
  availableChannelMembershipsPermissions,
  permissions,
};
