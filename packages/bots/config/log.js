const package$ = require('../package');

/**
 * Logs Levels which are possible:
 * trace,
 * debug,
 * info,
 * warn,
 * error.
 */

module.exports = {
  app: {
    name: process.env.LOG_APP_NAME || 'foretold-bots',
    version: package$.version || '1.0.0',
  },
  remote_auth: {
    keyFilename: `${__dirname}/google.json`,
  },
  local_level: process.env.LOG_LEVEL_LOCAL || 'trace',
  remote_level: process.env.LOG_LEVEL_REMOTE || 'trace',
};
