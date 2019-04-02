const data = require('../data');

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function all(root, args, context, info) {
  const agentId = context.user.agentId;
  return await data.measurementData.getAll({ agentId });
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function one(root, args, context, info) {
  const agentId = context.user.agentId;
  return await data.measurementData.getOne(args.id, { agentId });
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function create(root, args, context, info) {
  const user = context.user;
  return await data.measurementData.createOne(args, user);
}

module.exports = {
  one,
  all,
  create,
};
