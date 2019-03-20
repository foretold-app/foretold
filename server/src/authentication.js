const _ = require('lodash');
const jwt = require('jsonwebtoken');

const { usersData } = require('./data');

/**
 * @param {Request} req
 * @return {string | null}
 */
function getToken(req) {
  if (req.headers.authorization && req.headers.authorization.split(' ')[0] === 'Bearer') {
    return req.headers.authorization.split(' ')[1];
  } else if (req.query && req.query.token) {
    return req.query.token;
  }
  return null;
}

/**
 * @param {string} token
 * @return {Promise<boolean | Model>}
 */
async function authenticationByJwtToken(token) {
  try {
    const decoded = jwt.verify(token, process.env.AUTH0_SECRET);
    if (!decoded.sub) throw new Error('No User Id');
    return await usersData.getUserByAuth0Id(decoded.sub);
  } catch (err) {
    throw err;
  }
}

/**
 * @param {Request} options
 * @return {Promise<Model | object>}
 */
async function authentication(options) {
  try {
    const token = getToken(options);
    if (token) {
      return await authenticationByJwtToken(token);
    }
    return {};
  } catch (err) {
    console.error('Authentication Error', err);
    throw err;
  }
}

module.exports = {
  authentication,
};

