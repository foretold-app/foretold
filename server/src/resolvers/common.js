/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function iAmOwner(root, args, context, info) {
  return true;
}

module.exports = {
  iAmOwner,
};
