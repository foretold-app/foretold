const authorizers = require('../authorizers');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function all(root, args, context, info) {
  return authorizers.availableAll(root, args, context, info);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function channelMutations(root, args, context, info) {
  return authorizers.availableChannelMutations(root, args, context, info);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function channelMembershipsMutations(root, args, context, info) {
  return authorizers.availableChannelMembershipsMutations(root, args, context, info);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function measurablesPermissions(root, args, context, info) {
  return authorizers.availableMeasurablesPermissions(root, args, context, info);
}

module.exports = {
  all,
  channelMutations,
  measurablesPermissions,
  channelMembershipsMutations,
};
