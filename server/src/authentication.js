const jwt = require('jsonwebtoken');

const { users } = require('./data');
const { agents } = require('./data');

const AUTH0_SECRET = process.env.AUTH0_SECRET;
const JWT_SECRET = process.env.JWT_SECRET;
const JWT_ISSUER = process.env.JWT_ISSUER || 'Foretold';
const JWT_EXPIN = process.env.JWT_EXPIN || '31 days';

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
 * @return {Promise<boolean>}
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
 * @return {Promise<boolean>}
 */
function decodeJwtToken(token) {
  try {
    return jwt.verify(token, JWT_SECRET);
  } catch (err) {
    throw err;
  }
}

/**
 * @param {object} payload
 * @param {string} subject
 * @return {string}
 */
function encodeJWT(payload, subject) {
  return jwt.sign(payload, JWT_SECRET, {
    expiresIn: JWT_EXPIN,
    issuer: JWT_ISSUER,
    subject,
  });
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
 * @return {Promise<Schema.Context>}
 */
async function authenticationByJwtToken(token) {
  try {
    const decoded = decodeJwtToken(token);
    const agentId = decoded.sub;
    if (!agentId) throw new Error('No Agent Id');

    const agent = await agents.getOne(agentId);
    if (!agent) throw new Error('Not authenticated');

    const bot = await agent.getBot();
    const user = await agent.getUser();
    const creator = bot || user;

    return { agent, bot, user, creator };
  } catch (err) {
    throw err;
  }
}

/**
 * @param {string} token
 * @return {Promise<string>}
 */
async function getJwtByAuth0Jwt(token) {
  try {
    const user = await authenticationByAuth0JwtToken(token);
    const agentId = user.agentId;
    const payload = {};
    return encodeJWT(payload, agentId);
  } catch (err) {
    throw err;
  }
}

/**
 * @param {string} agentId
 * @return {Promise<string>}
 */
async function getJwtByAgentId(agentId) {
  try {
    const payload = {};
    return encodeJWT(payload, agentId);
  } catch (err) {
    throw err;
  }
}

/**
 * @param {Request} options
 * @return {Promise<*>}
 */
async function authentication(options) {
  try {
    const token = getQueryToken(options);
    if (token) {
      return await authenticationByJwtToken(token);
    }
    return null;
  } catch (err) {
    console.error('Authentication', err);
    throw err;
  }
}

module.exports = {
  getQueryToken,
  authenticationByAuth0JwtToken,
  getJwtByAuth0Jwt,
  getJwtByAgentId,
  authentication,
};

