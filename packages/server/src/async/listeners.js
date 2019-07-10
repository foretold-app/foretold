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

function runListeners() {
  try {
    emitter.on(
      events.MEASURABLE_STATE_TRANSITIONS,
      toJudgementPendingTransition,
    );
  } catch (e) {
    console.error('Listener error', e);
  }
}

module.exports = {
  runListeners,
};
