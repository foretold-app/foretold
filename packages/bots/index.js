const { runListeners } = require('./sync/listeners');
const { runJobs } = require('./sync');

function run() {
  runListeners();
  runJobs();
}

module.exports = {
  run,
  runJobs,
  runListeners,
};