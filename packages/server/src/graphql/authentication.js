const _ = require('lodash');

const data = require('../data');

/**
 * @param {Request} req
 * @return {string | null}
 */
function getQueryToken(req) {
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
    const token = getQueryToken(options);
    if (token) {
      return await data.authentication.authenticate(token);
    }
    return null;
  } catch (err) {
    console.error('Authentication', err);
    throw err;
  }
}

module.exports = {
  getQueryToken,
  authentication,
};
