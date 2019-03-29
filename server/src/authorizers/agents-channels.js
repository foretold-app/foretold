const _ = require('lodash');
const { rule } = require('graphql-shield');

const models = require('../models');

/**
 * @param {string} roleName
 */
function roleRule(roleName) {
  /**
   * @param {*} root
   * @param {object} args
   * @param {Schema.Context} context
   * @return {Promise<boolean>}
   */
  return (root, args, context) => {
    const role = _.get(context, 'agentsChannelsRole', []);
    console.log(`\x1b[33m Rule (roleRule) role "${role}" = "${roleName}" \x1b[0m`);
    return role === roleName;
  };
}

const isAdminRule = roleRule(models.AgentsChannels.ROLE.ADMIN);
const isViewerRule = roleRule(models.AgentsChannels.ROLE.VIEWER);

/** @type {Rule} */
const isAdmin = rule()(isAdminRule);
/** @type {Rule} */
const isViewer = rule()(isViewerRule);

module.exports = {
  isAdmin,
  isAdminRule,
  isViewer,
};
