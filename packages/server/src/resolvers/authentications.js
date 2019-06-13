const _ = require('lodash');

const data = require('../data');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<{jwt: string}>}
 */
async function getJwtByAuth0Jwt(root, args, context, info) {
  const auth0jwt = _.get(args, 'auth0jwt');
  const jwt = await data.authentication.getJwtByAuth0Jwt(auth0jwt);
  return { jwt };
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function getJwtByAgentId(root, args, context, info) {
  const botAgentId = _.get(root, 'agentId');
  return await data.authentication.getJwtForever(botAgentId);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function getTokenByAgentId(root, args, context, info) {
  const agentId = _.get(root, 'agentId');
  return await data.tokens.getOrCreateActiveTokenForAgentId(agentId);
}

module.exports = {
  getJwtByAuth0Jwt,
  getJwtByAgentId,
  getTokenByAgentId,
};
