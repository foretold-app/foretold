const cron = require('node-schedule');

const emitter = require('./emitter');
const events = require('./events');

function runJobs() {
  console.log('Jobs are in a queue.');

  const EVERY_HOUR = '0 * * * *';

  const job = cron.scheduleJob(EVERY_HOUR, () => {
    emitter.emit(events.MEASURABLE_STATE_TRANSITIONS);
    emitter.emit(events.AGGREGATION_BOT);
  });

  return {
    job,
  };
}

module.exports = {
  runJobs,
};
