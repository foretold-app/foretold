const _ = require('lodash');

const ERR_1 = () => 'Either Auth0 JWT and Auth0 Access Token should be not '
  + 'empty or Auth Token.';

/**
 * @param {*} root
 * @param {object} args
 * @param {string} args.auth0jwt
 * @param {string} args.auth0accessToken
 * @param {string} args.authToken
 * @param {Schema.Context} _context
 * @param {object} _info
 * @return {Promise<boolean>}
 */
async function authenticationInputValidation(root, args, _context, _info) {
  const auth0jwt = _.get(args, 'auth0jwt', null);
  const auth0accessToken = _.get(args, 'auth0accessToken', null);
  const authToken = _.get(args, 'authToken', null);

  if (!_.isEmpty(auth0jwt) && !_.isEmpty(auth0accessToken)) return true;
  if (!_.isEmpty(authToken)) return true;

  throw new Error(ERR_1());
}

module.exports = {
  authenticationInputValidation,
};
