const emitter = require('./emitter');
const events = require('./events');

const measurables = require('./measurables');

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

async function measurableState(_measurableInstance) {
  const name = 'Job::measurableState';
  console.log(name);

  try {
    const result = true;
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

async function sendEmails() {
  const name = 'Job::sendEmails';
  console.log(name);

  try {
    const result = true;
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
    emitter.on(events.MEASURABLE_STATE_IS_CHANGED, measurableState);
  } catch (e) {
    console.error('Listener error', e);
  }
}

module.exports = {
  runListeners,
};
