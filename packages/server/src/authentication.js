const _ = require('lodash');

const data = require('./data');

/**
 * @param {Request} req
 * @return {string | null}
 */
function getQueryToken(req) {
  const authorization = _.get(req, 'headers.authorization');
  const token = _.get(req, 'query.token');

  if (authorization && authorization.split(' ')[0] === 'Bearer') {
    return authorization.split(' ')[1];
  } else if (!!token) {
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

