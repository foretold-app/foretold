/**
 * @param {object | null} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function authentication(root, args, context, info) {
  return { jwt: 'str' };
}

module.exports = {
  authentication,
};
