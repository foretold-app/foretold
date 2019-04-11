const { rules } = require('../authorizers');

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


module.exports = {
  all,
};
