const emitter = require('./emitter');
const events = require('./events');

const triggers = require('./triggers');

function measurablesStates() {
  try {
    console.log('measurablesStates');
    new triggers.MeasurablesStates().main().then((result) => {
      console.log('measurablesStates', 'all done', result);
    }).catch((err) => {
      console.error('measurablesStates', err.message, data);
      console.error(err);
    });
  } catch (e) {
    console.log('memberPostSave error');
    console.error(e);
  }
}

function listen() {
  try {
    emitter.on(events.MEASURABLES_STATES, measurablesStates);
  } catch (e) {
    console.log('Listener error');
    console.error(e);
  }
}

module.exports = {
  listen,
};
