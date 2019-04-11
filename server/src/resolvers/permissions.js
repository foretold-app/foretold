const _ = require('lodash');
const { rules, rulesChannel } = require('../authorizers');
const { channelMemberships } = require('../middlewares/channel-memberships');
const { channelByRoot } = require('../middlewares/channels');

/**
 * @param {object} rules
 * @return {function(*=, *=, *=, *=): {allow: Array, deny: Array}}
 */
function getList(rules) {
  return async (root, args, context, info) => {
    const allow = [];
    const deny = [];

    for (const ruleName in rules) {
      const rule = rules[ruleName];
      const resolving = await rule.resolve(root, args, context, info);
      if (resolving === true) {
        allow.push(ruleName);
      } else {
        deny.push(ruleName);
      }
    }

    _.pull(allow, '*');
    _.pull(deny, '*');

    return { allow, deny };
  };
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
async function all(root, args, context, info) {
  const mutations = await (getList(rules.Mutation))(root, args, context, info);
  const queries = await (getList(rules.Query))(root, args, context, info);
  return { mutations, queries };
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
async function availableChannelMutations(root, args, context, info) {
  root = _.cloneDeep(root);
  args = _.cloneDeep(args);
  context = _.cloneDeep(context);
  info = _.cloneDeep(info);
  await channelByRoot(root, args, context, info);
  await channelMemberships(root, args, context, info);
  const mutations = await (getList(rulesChannel.Mutation))(root, args, context, info);
  return mutations.allow;
}

module.exports = {
  all,
  availableChannelMutations,
};
