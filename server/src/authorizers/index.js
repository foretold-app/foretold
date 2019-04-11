const { getPermissions, rules, rulesChannel, rulesChannelMemberships } = require('./permissions');

const permissions = getPermissions();

module.exports = {
  rules,
  rulesChannel,
  rulesChannelMemberships,
  permissions,
};
