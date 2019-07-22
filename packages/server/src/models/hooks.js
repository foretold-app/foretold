const events = require('../async/events');
const emitter = require('../async/emitter');

const { MEASURABLE_STATE } = require('./enums/measurable-state');

function addHooks(db) {
  db.sequelize.addHook('afterUpdate', (instance) => {
    if (
      instance instanceof db.Measurable &&
      instance.changed('state')
    ) {
      emitter.emit(events.MEASURABLE_STATE_IS_CHANGED, instance);
    }

    if (
      instance instanceof db.Measurable &&
      instance.changed('state') &&
      instance.get('state') === MEASURABLE_STATE.JUDGED
    ) {
      // @todo: Do it only for the first time
      emitter.emit(events.MEASURABLE_STATE_IS_RESOLVED, instance);
    }

  });
}

module.exports = {
  addHooks,
};
