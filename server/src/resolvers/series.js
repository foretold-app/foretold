const { resolver } = require("graphql-sequelize");

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
async function one(root, args, context, info) {
  return await resolver(models.Series)(root, args, context, info);
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function all(root, args, context, info) {
  return await resolver(models.Series)(root, args, context, info);
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function create(root, args, context, info) {
  return await data.seriesData.createSeries(root, args, context);
}

module.exports = {
  create,
  one: authorizerChannelAfter(one),
  all: authorizerChannelByArg(all),
};
