const { runListeners } = require('./sync/listeners');
const { runJobs } = require('./sync');
const logger = require('./log');

const log = logger.module('bots');

function run() {
  log.info('Bots module starts.');
  runListeners();
  runJobs();
}

module.exports = {
  run,
  runJobs,
  runListeners,
};