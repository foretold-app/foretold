const _ = require('lodash');
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
  return await data.measurablesData.createOne(datas, user);
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function archive(root, args, context, info) {
  const { id } = args;
  const user = context.user;
  return await data.measurablesData.archive(id, user);
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function unarchive(root, args, context, info) {
  const { id } = args;
  const user = context.user;
  return await data.measurablesData.unArchive(id, user);
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function update(root, args, context, info) {
  const id = args.id;
  const datas = _.omit(args, ['id']);
  const user = context.user;
  return await data.measurablesData.updateOne(id, datas, user);
}


module.exports = {
  one,
  all,
  update,
  unarchive,
  archive,
  create,
};
