const { runJobs } = require('./jobs');
const { runListeners } = require('./schedule');

const events = require('./events');
const emitter = require('./emitter');

function run() {
  runJobs();
  runListeners();
}

module.exports = {
  run,
  runJobs,
  runListeners,

  events,
  emitter,
};
