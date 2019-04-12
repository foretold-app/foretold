const jwt = require('jsonwebtoken');

const { users } = require('./data');

const AUTH0_SECRET = process.env.AUTH0_SECRET;
const JWT_SECRET = process.env.JWT_SECRET;
const JWT_ISSUER = process.env.JWT_ISSUER || 'Foretold';
const JWT_EXPIN = process.env.JWT_EXPIN || '1m';

if (!AUTH0_SECRET) throw new ReferenceError('AUTH0_SECRET is not defined');
if (!JWT_SECRET) throw new ReferenceError('JWT_SECRET is not defined');
if (!JWT_ISSUER) throw new ReferenceError('JWT_ISSUER is not defined');
if (!JWT_EXPIN) throw new ReferenceError('JWT_EXPIN is not defined');

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
 * @param {string} token
 * @return {Promise<boolean | Models.User>}
 */
function decodeAuth0JwtToken(token) {
  try {
    return jwt.verify(token, AUTH0_SECRET);
  } catch (err) {
    throw err;
  }
}

/**
 * @param {string} token
 * @return {Promise<boolean | Models.User>}
 */
async function authenticationByAuth0JwtToken(token) {
  try {
    const decoded = decodeAuth0JwtToken(token);
    if (!decoded.sub) throw new Error('No User Id');
    return await users.getUserByAuth0Id(decoded.sub);
  } catch (err) {
    throw err;
  }
}

/**
 * @param {string} token
 * @return {Promise<boolean | Models.User>}
 */
async function getJwtByAuth0Jwt(token) {
  try {
    const user = await authenticationByAuth0JwtToken(token);
    const agentId = user.agentId;
    const payload = {};
    return jwt.sign(payload, JWT_SECRET, {
      expiresIn: JWT_EXPIN,
      issuer: JWT_ISSUER,
      subject: agentId,
    });
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
    const token = getQueryToken(options);
    if (token) {
      return await authenticationByAuth0JwtToken(token);
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
  getJwtByAuth0Jwt,
  authenticationByAuth0JwtToken,
};

