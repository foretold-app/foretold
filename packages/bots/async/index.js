const cron = require('node-schedule');

const emitter = require('./emitter');
const events = require('./events');

const config = require('../config');

function runJobs() {
  console.log('Jobs are in a queue.');
  console.log('Aggregation schedule:', config.AGGR_SCHEDULE);

  const job = cron.scheduleJob(config.AGGR_SCHEDULE, () => {
    emitter.emit(events.AGGREGATION_BOT);
  });

  return {
    job,
  };
}

module.exports = {
  runJobs,
};
