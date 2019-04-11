const { getPermissions, rules } = require('./permissions');

const permissions = getPermissions();

module.exports = {
  rules,
  permissions,
};
