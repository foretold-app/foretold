const _ = require('lodash');

const { getJwtByAuth0Jwt } = require('../authentication');

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

module.exports = {
  authentication,
};
