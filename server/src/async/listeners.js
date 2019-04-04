const emitter = require('./emitter');
const events = require('./events');

const triggers = require('./triggers');

function measurablesStates() {
  try {
    console.log('Job::measurablesStates');
    new triggers.MeasurablesStates().main().then((result) => {
      console.log('Job::measurablesStates', 'all done', result);
    }).catch((err) => {
      console.error('Job::measurablesStates', err.message);
      console.error(err);
    });
  } catch (e) {
    console.log('Job::measurablesStates error');
    console.error(e);
  }
}

function runListeners() {
  try {
    emitter.on(events.MEASURABLES_STATES, measurablesStates);
  } catch (e) {
    console.log('Listener error');
    console.error(e);
  }
}

module.exports = {
  runListeners,
};
