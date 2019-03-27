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
  return await data.measurablesData.getAll({ ...args, agentId });
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
  return await data.measurablesData.getOne(args.id, { agentId });
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
  const datas = {
    ...args,
    creatorId: user.agentId,
  };
  return await data.measurablesData.createMeasurable(datas, user);
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function archive(root, args, context, info) {
  return await data.measurablesData.archiveMeasurable(root, args, context);
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function unarchive(root, args, context, info) {
  return await data.measurablesData.unArchiveMeasurable(root, args, context);
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function edit(root, args, context, info) {
  return await data.measurablesData.editMeasurable(root, args, context);
}


module.exports = {
  one,
  all,
  edit,
  unarchive,
  archive,
  create,
};
