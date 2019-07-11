const emitter = require('./emitter');
const events = require('./events');

const measurables = require('./measurables');
const producers = require('./producers');
const consumers = require('./consumers');

async function toJudgementPendingTransition() {
  const name = 'Job::toJudgementPendingTransition';
  console.log(name);

  try {
    const reducer = new measurables.Reducer();
    const result = await reducer.toJudgementPending();
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

async function measurableState(measurableInstance) {
  const name = 'Job::measurableState';
  console.log(name);

  try {
    const producer = new producers.MeasurableState(measurableInstance);
    const result = await producer.main();
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

async function sendEmails() {
  const name = '\x1b[35mJob::sendEmails\x1b[0m';
  console.log(name);

  try {
    const consumer = new consumers.Emails();
    const result = await consumer.main();
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

function runListeners() {
  try {
    emitter.on(events.EVERY_HOUR, toJudgementPendingTransition);
    emitter.on(events.EVERY_TEN_MINUTES, sendEmails);
    emitter.on(events.EVERY_MINUTE, sendEmails);
    emitter.on(events.EVERY_MINUTE, sendEmails);
    emitter.on(events.EVERY_MINUTE, sendEmails);
    emitter.on(events.MEASURABLE_STATE_IS_CHANGED, measurableState);
  } catch (e) {
    console.error('Listener error', e);
  }
}

module.exports = {
  runListeners,
};
