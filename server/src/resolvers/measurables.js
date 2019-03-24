const { resolver } = require("graphql-sequelize");

const data = require('../data');
const models = require('../models');

const { authorizerChannel } = require('../authorizers/channels');

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

module.exports = {
  one,
  all: authorizerChannel(all),
};
