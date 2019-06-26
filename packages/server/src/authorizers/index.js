const { getPermissions } = require('./permissions');

const availability = require('./availability');

const permissions = getPermissions();

module.exports = {
  ...availability,
  permissions,
};
