const emitter = require('./emitter');
const events = require('./events');

const measurables = require('./measurables');
const producers = require('./producers');
const consumers = require('./consumers');
const { Mailer } = require('./mailer');

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

async function measurableStateIsChanged(measurableInstance) {
  const name = 'Job::measurableStateIsChanged';
  console.log(name);

  try {
    const producer = new producers.MeasurableStateChanged(measurableInstance);
    const result = await producer.main();
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

async function measurableStateIsResolved(measurableInstance) {
  const name = 'Job::measurableStateIsResolved';
  console.log(name);

  try {
    const producer = new producers.MeasurableStateResolved(measurableInstance);
    const result = await producer.main();
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

async function emailConsumer() {
  const name = '\x1b[35mJob::emailConsumer\x1b[0m';
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

async function mailer(envelop = {}) {
  const name = '\x1b[35mJob::mailer\x1b[0m';
  console.log(name);

  try {
    const result = await new Mailer(envelop).main();
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

function runListeners() {
  try {
    emitter.on(events.EVERY_HOUR, toJudgementPendingTransition);
    emitter.on(events.EVERY_MINUTE, emailConsumer);
    emitter.on(events.MAIL, mailer);
    emitter.on(events.MEASURABLE_STATE_IS_CHANGED, measurableStateIsChanged);
    emitter.on(events.MEASURABLE_STATE_IS_RESOLVED, measurableStateIsResolved);
  } catch (e) {
    console.error('Listener error', e);
  }
}

module.exports = {
  runListeners,
};
