const _ = require('lodash');

/**
 * @param {*} root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<boolean>}
 */
async function iAmOwner(root, _args, context, _info) {
  const creatorId = _.get(root, 'creatorId') || _.get(root, 'agentId');
  const currentAgentId = _.get(context, 'agent.id');
  return !!creatorId && creatorId === currentAgentId;
}

/**
 * @todo: remove then
 * @param {*} root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<boolean>}
 */
async function iAmOwnerByUserId(root, _args, context, _info) {
  const creatorId = _.get(root, 'userId');
  const currentCreatorId = _.get(context, 'user.id');
  return !!creatorId && creatorId === currentCreatorId;
}

/**
 * @param {*} root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<boolean>}
 */
async function isMe(root, _args, context, _info) {
  const agentId = _.get(root, 'agentId') || _.get(root, 'id');
  const currentAgentId = _.get(context, 'agent.id');
  return !!agentId && agentId === currentAgentId;
}

module.exports = {
  isMe,
  iAmOwner,
  iAmOwnerByUserId,
};
