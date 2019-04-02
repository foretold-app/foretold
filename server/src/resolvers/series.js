const data = require('../data');

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function one(root, args, context, info) {
  const agentId = context.user.agentId;
  return await data.series.getOne(args.id, { agentId });
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function all(root, args, context, info) {
  const agentId = context.user.agentId;
  return await data.series.getAll({ ...args, agentId });
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function create(root, args, context, info) {
  const datas = {
    ...args,
    creatorId: context.user.agentId,
  };
  return await data.series.createOne(datas);
}

module.exports = {
  create,
  one,
  all,
};
