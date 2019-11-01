const _ = require('lodash');

/**
 * @param {*} root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<boolean>}
 */
async function iAmOwner(root, _args, context, _info) {
  const creatorId =
    _.get(root, 'creatorId', null) || _.get(root, 'agentId', null);
  const currentAgentId = _.get(context, 'agent.id', null);
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
  const creatorId = _.get(root, 'userId', null);
  const currentCreatorId = _.get(context, 'user.id', null);
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
  const agentId = _.get(root, 'agentId', null) || _.get(root, 'id', null);
  const currentAgentId = _.get(context, 'agent.id', null);
  return !!agentId && agentId === currentAgentId;
}

module.exports = {
  isMe,
  iAmOwner,
  iAmOwnerByUserId,
};
