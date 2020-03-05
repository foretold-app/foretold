jest.mock('./data/models/index');
jest.mock('./data/index', () => require('./data/__mocks__/data'));
jest.mock('./lib/slack');
jest.mock('./lib/log', () => require('./lib/log/__mocks__/log'));
jest.mock('./data/models/index', () => require('./data/models/__mocks__/models'));
jest.mock('@foretold/prediction-analysis', () => require('./__mocks__/prediction-analysis'));

process.env.SERVER_URL = 'SERVER_URL1';

process.env.AUTH0_SECRET = 'AUTH0_SECRET1';
process.env.AUTH0_DOMAIN = 'AUTH0_DOMAIN1';
process.env.AUTH0_CLIENT_ID = 'AUTH0_CLIENT_ID1';

process.env.JWT_SECRET = 'JWT_SECRET1';
process.env.JWT_ISSUER = 'JWT_ISSUER1';
process.env.JWT_EXPIN = 'JWT_EXPIN1';
