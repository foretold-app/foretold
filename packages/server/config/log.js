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
    name: package$.name || 'app',
    version: package$.version || '1.0.0',
  },
  remote_auth: {
    // projectId: 'my-google-cloud-project',
    keyFilename: process.env.LOG_STACKDRIVER_AUTH
      || '/secrets/stackdriver/gcloud-auth.json',
  },
  local_level: process.env.LOG_LEVEL_LOCAL,
  remote_level: process.env.LOG_LEVEL_REMOTE,
};
