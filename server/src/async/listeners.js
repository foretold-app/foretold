const emitter = require('./emitter');
const events = require('./events');

const measurables = require('./measurables');

function toJudgementPendingTransition() {
  try {
    console.log('Job::toJudgementPendingTransition');
    new measurables.ToJudgementPending().main().then((result) => {
      console.log('Job::toJudgementPendingTransition', 'all done', result);
    }).catch((err) => {
      console.error('Job::toJudgementPendingTransition', err.message);
      console.error(err);
    });
  } catch (e) {
    console.log('Job::toJudgementPendingTransition error');
    console.error(e);
  }
}

function runListeners() {
  try {
    emitter.on(events.MEASURABLE_STATE_TRANSITIONS, toJudgementPendingTransition);
  } catch (e) {
    console.log('Listener error');
    console.error(e);
  }
}

module.exports = {
  runListeners,
};
