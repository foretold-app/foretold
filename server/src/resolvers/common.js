const _ = require('lodash');

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function iAmOwner(root, args, context, info) {
  const creatorId = _.get(root, 'creatorId') || _.get(root, 'agentId');
  const userAgentId = _.get(context, 'user.agentId');
  return creatorId && creatorId === userAgentId;
}

module.exports = {
  iAmOwner,
};
