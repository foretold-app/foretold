const cron = require('node-schedule');

const logger = require('../log');

const emitter = require('./emitter');
const events = require('./events');

const config = require('../config');

const log = logger.module('bots/listeners');

function runJobs() {
  log.trace('Jobs are in a queue.');
  log.trace('Aggregation schedule:', config.AGGR_SCHEDULE);

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
