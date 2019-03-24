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
function all(root, args, context, info) {
  return resolver(models.Series);
}

module.exports = {
  all: authorizerChannel(all),
};
