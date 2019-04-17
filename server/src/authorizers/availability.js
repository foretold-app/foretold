const _ = require('lodash');

const { rules } = require('./permissions');
const { rulesChannelMemberships } = require('./permissions');
const { rulesChannel } = require('./permissions');
const { rulesMeasurables } = require('./permissions');

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
 * @param {object} rulesPart
 */
function getAll(rulesPart) {
  /**
   * @param {object | null} root
   * @param {object} args
   * @param {Schema.Context} context
   * @param {object} info
   * @returns {Promise<*>}
   */
  return async (root, args, context, info) => {
    const mutations = await (getList(rulesPart.Mutation))(root, args, context, info);
    const queries = await (getList(rulesPart.Query))(root, args, context, info);
    return { mutations, queries };
  };
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function availableAll(root, args, context, info) {
  return getAll(rules)(root, args, context, info);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function availableChannelMutations(root, args, context, info) {
  return getAll(rulesChannel)(root, args, context, info);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function availableChannelMembershipsMutations(root, args, context, info) {
  return getAll(rulesChannelMemberships)(root, args, context, info);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function availableMeasurablesPermissions(root, args, context, info) {
  return getAll(rulesMeasurables)(root, args, context, info);
}

module.exports = {
  availableAll,
  availableChannelMutations,
  availableMeasurablesPermissions,
  availableChannelMembershipsMutations,
};
