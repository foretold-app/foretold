const logger = require('../lib/log');

const emitter = require('./emitter');
const events = require('./events');
const producers = require('./producers');

const actions = require('./actions');
const consumers = require('./consumers');
const { Mailer } = require('./mailer');
const { GitHubApi } = require('./github/git-hub-api');

const log = logger.module('sync/listeners');

/**
 * @param {function} Producer
 * @param {string} method
 * @returns {function(*=): boolean}
 */
function listenFor(Producer, method = 'main') {
  const name = `${Producer.name}`;
  log.trace(`Listen: ${name}.${method}.`);

  return async function listenForProducer(input) {
    log.trace(`Run listener: ${name}.`);

    try {
      const job = new Producer(input);
      const result = await job[method](input);
      log.trace(name, 'all done', result);
    } catch (e) {
      console.error(name, e.message, e);
    }

    return true;
  };
}

function runListeners() {
  console.log('Listeners are in a queue.');
  try {
    emitter.on(events.SERVER_IS_READY,
      listenFor(GitHubApi, 'addHook'));

    emitter.on(events.EVERY_HOUR,
      listenFor(actions.MeasurablesStateMachine, 'toJudgementPending'));
    emitter.on(events.EVERY_HOUR,
      listenFor(actions.MeasurablesStateMachine, 'toResolving'));
    emitter.on(events.EVERY_MINUTE,
      listenFor(consumers.Emails));
    emitter.on(events.EVERY_MINUTE,
      listenFor(actions.UserUpdater));
    emitter.on(events.EVERY_HOUR,
      listenFor(actions.MaterializedViewsUpdater));
    emitter.on(events.MAIL,
      listenFor(Mailer));

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
    //   listenFor(actions.Notifications, 'updateMeasurableSlackNotification'));

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
    //   listenFor(actions.Notifications, 'newMeasurementSlackNotification'));
    emitter.on(events.NEW_MEASUREMENT,
      listenFor(actions.MeasurablesStateMachine, 'measurableStateTransition'));

    /**
     * NEW_MEASURABLE
     */
    emitter.on(events.NEW_MEASURABLE,
      listenFor(producers.feedItems.NewMeasurable));
    // emitter.on(events.NEW_MEASURABLE,
    //   listenFor(actions.Notifications, 'newMeasurableSlackNotification'));

    /**
     * NEW_CHANNEL
     */
    emitter.on(events.NEW_CHANNEL,
      listenFor(producers.feedItems.NewChannel));
    emitter.on(events.NEW_CHANNEL,
      listenFor(actions.Creators, 'createChannelMembership'));

    /**
     * Common.
     */
    emitter.on(events.NEW_SERIES,
      listenFor(actions.Creators, 'createMeasurables'));
    emitter.on(events.CREATING_BOT,
      listenFor(actions.Creators, 'createBotAgent'));
    emitter.on(events.CREATING_USER,
      listenFor(actions.Creators, 'createUserAgent'));
    emitter.on(events.UPDATING_MEASURABLE,
      listenFor(actions.Creators, 'checkMeasurableState'));
    emitter.on(events.VALIDATING_MEASUREMENT,
      listenFor(actions.Creators, 'checkMeasurement'));
    emitter.on(events.NEW_INVITATION,
      listenFor(producers.notifications.Invitation));
    emitter.on(events.NEW_USER,
      listenFor(actions.Invitations, 'transition'));
    emitter.on(events.USER_CHANGED,
      listenFor(actions.Invitations, 'transition'));
    emitter.on(events.USER_CHANGED,
      listenFor(actions.UserUpdater, 'updateUser'));

    emitter.on(events.NEW_NOTEBOOK,
      listenFor(producers.feedItems.NewNotebook));
  } catch (e) {
    console.error('Listener error', e);
  }
}

module.exports = {
  runListeners,
};
