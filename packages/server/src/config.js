const NODE_ENV = process.env.NODE_ENV;

const AUTH0_SECRET = process.env.AUTH0_SECRET;
const AUTH0_DOMAIN = process.env.AUTH0_DOMAIN;
const AUTH0_CLIENT_ID = process.env.AUTH0_CLIENT_ID;

const JWT_SECRET = process.env.JWT_SECRET;
const JWT_ISSUER = process.env.JWT_ISSUER || 'Foretold';
const JWT_EXPIN = process.env.JWT_EXPIN || '31 days';

const MAX_BOT_TOKEN_SIZE = process.env.MAX_BOT_TOKEN_SIZE * 1 || 32;

const GITHUB_PERSONAL_ACCESS_TOKEN = process.env.GITHUB_PERSONAL_ACCESS_TOKEN;
const GITHUB_REPO_OWNER = process.env.GITHUB_REPO_OWNER;
const GITHUB_REPO_NAME = process.env.GITHUB_REPO_NAME;
const GITHUB_WEBHOOK_SECRET = process.env.GITHUB_WEBHOOK_SECRET;

if (!AUTH0_SECRET) throw new ReferenceError('AUTH0_SECRET is not defined');
if (!JWT_SECRET) throw new ReferenceError('JWT_SECRET is not defined');
if (!JWT_ISSUER) throw new ReferenceError('JWT_ISSUER is not defined');

if (!JWT_EXPIN) throw new ReferenceError('JWT_EXPIN is not defined');
if (!AUTH0_DOMAIN) throw new ReferenceError('AUTH0_DOMAIN is not defined');
if (!AUTH0_CLIENT_ID) throw new ReferenceError('AUTH0_CLIENT_ID is not defined');

const conf = {
  NODE_ENV,

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
};

if (NODE_ENV === 'development') {
  console.log('Config', conf);
}

module.exports = conf;
