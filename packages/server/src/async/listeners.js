const emitter = require('./emitter');
const events = require('./events');

const measurables = require('./measurables');

function toJudgementPendingTransition() {
  const name = 'Job::toJudgementPendingTransition';
  try {
    console.log(name);

    new measurables.ToJudgementPending().main().then((result) => {
      console.log(name, 'all done', result);
    }).catch((err) => {
      console.error(name, err.message, err);
    });

  } catch (e) {
    console.error(name, 'error', e);
  }
}

function runListeners() {
  try {
    emitter.on(events.MEASURABLE_STATE_TRANSITIONS, toJudgementPendingTransition);
  } catch (e) {
    console.error('Listener error', e);
  }
}

module.exports = {
  runListeners,
};
