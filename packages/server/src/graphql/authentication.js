const _ = require('lodash');

const { Authentication } = require('../lib/authentication');

/**
 * @param {Request} req
 * @return {string | null}
 */
function _getQueryToken(req) {
  const authorization = _.get(req, 'headers.authorization');
  const token = _.get(req, 'query.token');
  const authorization$ = _.split(authorization, ' ');
  const authorizationName = _.get(authorization$, 0);
  const authorizationBody = _.get(authorization$, 1);

  if (authorizationName === 'Bearer') {
    return authorizationBody;
  }

  if (!!token) {
    return token;
  }

  return null;
}

/**
 * @param {Request} options
 * @return {Promise<* | null>}
 */
async function authentication(options) {
  try {
    const token = _getQueryToken(options);

    if (token) {
      return await new Authentication().authenticate(token);
    }

    return null;
  } catch (err) {
    console.error('Authentication', err);
    throw err;
  }
}

module.exports = {
  authentication,
};
