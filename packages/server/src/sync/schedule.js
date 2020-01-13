const emitter = require('./emitter');
const events = require('./events');
const producers = require('./producers');
const listeners = require('./listeners');
const { listenFor } = require('./listeners');

function runListeners() {
  console.log('Listeners are in a queue.');
  try {
    emitter.on(events.SERVER_IS_READY,
      listeners.addGitHubWebHook);

    emitter.on(events.EVERY_HOUR,
      listeners.toJudgementPendingTransition);
    emitter.on(events.EVERY_HOUR,
      listeners.toResolving);
    emitter.on(events.EVERY_MINUTE,
      listeners.emailConsumer);
    emitter.on(events.EVERY_MINUTE,
      listeners.updateUsers);
    emitter.on(events.EVERY_HOUR,
      listeners.updateMaterializedViews);
    emitter.on(events.MAIL,
      listeners.mailer);

    /**
     * MEASURABLE_CHANGED
     */
    emitter.on(events.MEASURABLE_CHANGED,
      listenFor(producers.notifications.MeasurableStateChanged));
    emitter.on(events.MEASURABLE_CHANGED,
      listenFor(producers.notifications.MeasurableStateResolved));
    emitter.on(events.MEASURABLE_CHANGED,
      listenFor(producers.feedItems.NewMeasurableReachedResolution));
    // emitter.on(events.MEASURABLE_CHANGED,
    //   listeners.updateMeasurableSlackNotification);

    /**
     * NEW_MEMBERSHIP
     */
    emitter.on(events.NEW_MEMBERSHIP,
      listenFor(producers.notifications.MemberAddedToCommunity));
    emitter.on(events.NEW_MEMBERSHIP,
      listenFor(producers.feedItems.MemberJoinedCommunity));

    /**
     * NEW_MEASUREMENT
     */
    emitter.on(events.NEW_MEASUREMENT,
      listenFor(producers.feedItems.NewMeasurementPrediction));
    emitter.on(events.NEW_MEASUREMENT,
      listenFor(producers.feedItems.NewMeasurementComment));
    emitter.on(events.NEW_MEASUREMENT,
      listenFor(producers.feedItems.NewMeasurementResolution));
    emitter.on(events.NEW_MEASUREMENT,
      listenFor(producers.feedItems.NewMeasurementNotAvailable));
    // emitter.on(events.NEW_MEASUREMENT,
    //   listeners.newMeasurementSlackNotification);
    emitter.on(events.NEW_MEASUREMENT,
      listeners.measurableStateTransition);

    /**
     * NEW_MEASURABLE
     */
    emitter.on(events.NEW_MEASURABLE,
      listenFor(producers.feedItems.NewMeasurable));
    // emitter.on(events.NEW_MEASURABLE,
    //   listeners.newMeasurableSlackNotification);

    /**
     * NEW_CHANNEL
     */
    emitter.on(events.NEW_CHANNEL,
      listenFor(producers.feedItems.NewChannel));
    emitter.on(events.NEW_CHANNEL,
      listeners.createChannelMembership);

    /**
     * Common.
     */
    emitter.on(events.NEW_SERIES,
      listeners.createNewMeasurables);
    emitter.on(events.CREATING_BOT,
      listeners.createBotAgent);
    emitter.on(events.CREATING_USER,
      listeners.createUserAgent);
    emitter.on(events.UPDATING_MEASURABLE,
      listeners.checkMeasurable);
    emitter.on(events.VALIDATING_MEASUREMENT,
      listeners.checkMeasurement);
    emitter.on(events.NEW_INVITATION,
      listenFor(producers.notifications.Invitation));
    emitter.on(events.NEW_USER,
      listeners.invitations);
    emitter.on(events.USER_CHANGED,
      listeners.invitations);
    emitter.on(events.USER_CHANGED,
      listeners.updateUser);

    emitter.on(events.NEW_NOTEBOOK,
      listenFor(producers.feedItems.NewNotebook));
  } catch (e) {
    console.error('Listener error', e);
  }
}

module.exports = {
  runListeners,
};
