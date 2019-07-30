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
      emitter.emit(events.MEASURABLE_STATE_IS_RESOLVED, instance);
    }
  });

  db.sequelize.addHook('afterCreate', (instance) => {
    if (instance instanceof db.ChannelMemberships) {
      emitter.emit(events.MEMBER_ADDED_TO_COMMUNITY, instance);
    }
  });
}

module.exports = {
  addHooks,
};
