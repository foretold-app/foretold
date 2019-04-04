const cron = require('node-schedule');

const emitter = require('./emitter');
const events = require('./events');

function runJobs() {
  console.log('Jobs are in a queue.');

  const EVERY_DAY_AT_MIDNIGHT = '0 0 * * *';

  const job = cron.scheduleJob(EVERY_DAY_AT_MIDNIGHT, () => {
    emitter.emit(events.MEASURABLES_STATES);
  });

  return {
    job,
  };
}

module.exports = {
  runJobs,
};
