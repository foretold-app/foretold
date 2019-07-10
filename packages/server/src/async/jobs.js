const cron = require('node-schedule');

const emitter = require('./emitter');
const events = require('./events');

// m h d month weekDay
const EVERY_HOUR = '0 * * * *';
const EVERY_TEN_MINUTES = '*/10 * * * *';
const EVERY_MINUTE = '* * * * *';

function runJobs() {
  console.log('Jobs are in a queue.');

  const jobs = [];

  jobs.push(cron.scheduleJob(EVERY_HOUR, () => {
    emitter.emit(events.EVERY_HOUR);
  }));

  jobs.push(cron.scheduleJob(EVERY_TEN_MINUTES, () => {
    emitter.emit(events.EVERY_TEN_MINUTES);
  }));

  jobs.push(cron.scheduleJob(EVERY_MINUTE, () => {
    emitter.emit(events.EVERY_MINUTE);
  }));

  return jobs;
}

module.exports = {
  runJobs,
};
