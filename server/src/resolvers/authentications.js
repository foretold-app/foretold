const _ = require('lodash');

const { getJwtByAuth0Jwt, getJwtByAgentId } = require('../authentication');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function authentication(root, args, context, info) {
  const auth0jwt = _.get(args, 'auth0jwt');
  const jwt = await getJwtByAuth0Jwt(auth0jwt);
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
async function getJwtForBot(root, args, context, info) {
  const botAgentId = _.get(root, 'agentId');
  return await getJwtByAgentId(botAgentId);
}

module.exports = {
  authentication,
  getJwtForBot,
};
