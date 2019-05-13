const data = require('./data');

/**
 * @param {Request} req
 * @return {string | null}
 */
function getQueryToken(req) {
  if (req.headers.authorization && req.headers.authorization.split(' ')[0] === 'Bearer') {
    return req.headers.authorization.split(' ')[1];
  } else if (req.query && req.query.token) {
    return req.query.token;
  }
  return null;
}

/**
 * @param {Request} options
 * @return {Promise<*>}
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

