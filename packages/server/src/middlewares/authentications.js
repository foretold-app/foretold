const _ = require('lodash');

const ERR_1 = () => 'Either Auth0 JWT and Auth0 Access Token should be not ' +
  'empty or Auth Token.';
const ERR_2 = () => 'Auth0 JWT is invalid.';

const JWT_PATTERN = '^[A-Za-z0-9-_=]+\.[A-Za-z0-9-_=]+\.?[A-Za-z0-9-_.+/=]*$';

/**
 * @param {*} root
 * @param {object} args
 * @param {string} args.auth0jwt
 * @param {string} args.auth0accessToken
 * @param {string} args.authToken
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function authenticationInputValidation(root, args, context, info) {
  const auth0jwt = _.get(args, 'auth0jwt');
  const auth0accessToken = _.get(args, 'auth0accessToken');
  const authToken = _.get(args, 'authToken');

  if (!_.isEmpty(auth0jwt) && !_.isEmpty(auth0accessToken)) return true;
  if (_.isEmpty(authToken)) throw new Error(ERR_1());

  return true;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {string} args.auth0jwt
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function authenticationInputJwtValidation(root, args, context, info) {
  const auth0jwt = _.get(args, 'auth0jwt');

  if (_.isEmpty(auth0jwt)) return true;
  if (!(new RegExp(JWT_PATTERN).test(auth0jwt))) throw new Error(ERR_2());

  return true;
}


module.exports = {
  authenticationInputValidation,
  authenticationInputJwtValidation,
};
