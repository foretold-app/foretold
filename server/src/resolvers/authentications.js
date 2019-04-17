const _ = require('lodash');

const authentication = require('../authentication');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<{jwt: string}>}
 */
async function getJwtByAuth0Jwt(root, args, context, info) {
  const auth0jwt = _.get(args, 'auth0jwt');
  const jwt = await authentication.getJwtByAuth0Jwt(auth0jwt);
  return { jwt };
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function getJwtByAgentId(root, args, context, info) {
  const botAgentId = _.get(root, 'agentId');
  return await authentication.getJwtByAgentId(botAgentId);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function getJwtByOwnerAgentId(root, args, context, info) {
  const botOwnerAgentId = _.get(root, 'ownerAgentId');
  return await authentication.getJwtByAgentId(botOwnerAgentId);
}

module.exports = {
  getJwtByAuth0Jwt,
  getJwtByAgentId,
  getJwtByOwnerAgentId,
};
