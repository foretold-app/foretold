const PORT = process.env.PORT || 4000;
const { NODE_ENV } = process.env;
const { SERVER_URL } = process.env;
const API_URL = process.env.API_URL
  || `https://${process.env.HEROKU_APP_NAME}.herokuapp.com/graphql`;

const { AUTH0_SECRET } = process.env;
const { AUTH0_DOMAIN } = process.env;
const { AUTH0_CLIENT_ID } = process.env;

const { JWT_SECRET } = process.env;
const JWT_ISSUER = process.env.JWT_ISSUER || 'Foretold';
const JWT_EXPIN = process.env.JWT_EXPIN || '31 days';

const MAX_BOT_TOKEN_SIZE = process.env.MAX_BOT_TOKEN_SIZE * 1 || 32;

const { GITHUB_PERSONAL_ACCESS_TOKEN } = process.env;
const { GITHUB_REPO_OWNER } = process.env;
const { GITHUB_REPO_NAME } = process.env;
const { GITHUB_WEBHOOK_SECRET } = process.env;
const GITHUB_API_URL = process.env.GITHUB_API_URL
  || 'https://api.github.com';
const GITHUB_QUERY_USER_AGENT = process.env.GITHUB_QUERY_USER_AGENT
  || 'Foretold App';

const DEV = NODE_ENV === 'development';
const PROD = NODE_ENV === 'production';

if (!SERVER_URL) {
  throw new ReferenceError('SERVER_URL is not defined');
}
if (!AUTH0_SECRET) {
  throw new ReferenceError('AUTH0_SECRET is not defined');
}
if (!JWT_SECRET) {
  throw new ReferenceError('JWT_SECRET is not defined');
}
if (!JWT_ISSUER) {
  throw new ReferenceError('JWT_ISSUER is not defined');
}
if (!JWT_EXPIN) {
  throw new ReferenceError('JWT_EXPIN is not defined');
}
if (!AUTH0_DOMAIN) {
  throw new ReferenceError('AUTH0_DOMAIN is not defined');
}
if (!AUTH0_CLIENT_ID) {
  throw new ReferenceError('AUTH0_CLIENT_ID is not defined');
}

const conf = {
  PORT,
  NODE_ENV,
  SERVER_URL,
  API_URL,
  PROD,
  DEV,

  AUTH0_SECRET,
  AUTH0_DOMAIN,
  AUTH0_CLIENT_ID,

  JWT_SECRET,
  JWT_ISSUER,
  JWT_EXPIN,

  MAX_BOT_TOKEN_SIZE,

  GITHUB_PERSONAL_ACCESS_TOKEN,
  GITHUB_REPO_OWNER,
  GITHUB_REPO_NAME,
  GITHUB_WEBHOOK_SECRET,
  GITHUB_API_URL,
  GITHUB_QUERY_USER_AGENT,
};

if (NODE_ENV === 'development') {
  console.log('Config', conf);
}

module.exports = conf;
