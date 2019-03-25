const { resolver } = require("graphql-sequelize");

const data = require('../data');
const models = require('../models');

const { authorizerChannelByArg } = require('../authorizers/channels');
const { authorizerChannelAfter } = require('../authorizers/channels');

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function all(root, args, context, info) {
  return await data.measurablesData.getAll(args);
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function one(root, args, context, info) {
  return await resolver(models.Measurable)(root, args, context, info);
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
  const data = {
    ...args,
    creatorId: user.agentId,
  };
  return await data.measurablesData.createMeasurable(data);
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
  edit,
  unarchive,
  archive,
  create,
  one: authorizerChannelAfter(one),
  all: authorizerChannelByArg(all),
};
