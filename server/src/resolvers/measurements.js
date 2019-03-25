const { resolver } = require("graphql-sequelize");

const data = require('../data');
const models = require('../models');

const { authorizerChannelByArg } = require('../authorizers/channels');

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function all(root, args, context, info) {
  return await resolver(models.Measurement)(root, args, context, info);
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function one(root, args, context, info) {
  return await resolver(models.Measurement)(root, args, context, info);
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function create(root, args, context, info) {
  return await data.measurementData.createMeasurement(root, args, context);
}

module.exports = {
  one,
  create,
  all,
};
