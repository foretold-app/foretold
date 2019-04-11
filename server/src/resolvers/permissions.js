const _ = require('lodash');

const authorizers = require('../authorizers');

const { channelMemberships } = require('../middlewares/channel-memberships');
const { channelByRoot, channel } = require('../middlewares/channels');


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
  root = _.cloneDeep(root);
  args = _.cloneDeep(args);
  context = _.cloneDeep(context);
  info = _.cloneDeep(info);
  await channelByRoot(root, args, context, info);
  await channelMemberships(root, args, context, info);
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
  root = _.cloneDeep(root);
  args = _.cloneDeep(args);
  context = _.cloneDeep(context);
  info = _.cloneDeep(info);
  await channel(root, args, context, info);
  await channelMemberships(root, args, context, info);
  return authorizers.availableChannelMembershipsMutations(root, args, context, info);
}

module.exports = {
  all,
  channelMutations,
  channelMembershipsMutations,
};
