const _ = require('lodash');

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<boolean>}
 */
async function iAmOwner(root, args, context, info) {
  const creatorId = _.get(root, 'creatorId') || _.get(root, 'agentId');
  const currentAgentId = _.get(context, 'user.agentId');
  return !!creatorId && creatorId === currentAgentId;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<boolean>}
 */
async function isMe(root, args, context, info) {
  const agentId = _.get(root, 'agentId') || _.get(root, 'id');
  const currentAgentId = _.get(context, 'user.agentId');
  return !!agentId && agentId === currentAgentId;
}

module.exports = {
  isMe,
  iAmOwner,
};
