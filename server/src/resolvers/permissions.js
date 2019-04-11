const authorizers = require('../authorizers');

/**
 * @todo: Super hacky.
 * @param {object | null} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function all(root, args, context, info) {
  return authorizers.availableAll(root, args, context, info);
}

/**
 * @todo: Super hacky.
 * @param {object | null} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function channelMutations(root, args, context, info) {
  return authorizers.availableChannelMutations(root, args, context, info);
}

/**
 * @todo: Super hacky.
 * @param {object | null} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function channelMembershipsMutations(root, args, context, info) {
  return authorizers.availableChannelMembershipsMutations(root, args, context, info);
}

module.exports = {
  all,
  channelMutations,
  channelMembershipsMutations,
};
