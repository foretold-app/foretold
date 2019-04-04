const cron = require('node-schedule');

const emitter = require('./emitter');
const events = require('./events');

const EVERY_DAY_AT_MIDNIGHT = '0 0 * * *';

const job = cron.scheduleJob(EVERY_DAY_AT_MIDNIGHT, () => {
  emitter.emit(events.MEASURABLE_STATE);
});

module.exports = job;
