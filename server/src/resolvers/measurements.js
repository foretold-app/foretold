const { resolver } = require("graphql-sequelize");

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
  return await resolver(models.Measurement)(root, args, context, info);
}

module.exports = {
  all: authorizerChannel(all),
};
