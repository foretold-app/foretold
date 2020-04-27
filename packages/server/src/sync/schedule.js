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
function listen(Producer, method = 'main') {
  const name = `${Producer.name}`;
  log.trace(`Listen: ${name}.${method}.`);

  return async function listenForProducer(input) {
    log.trace(`Run: ${name}.${method}.`);

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
      listen(GitHubApi, 'addHook'));

    emitter.on(events.EVERY_HOUR,
      listen(actions.MeasurablesStateMachine, 'toJudgementPending'));
    emitter.on(events.EVERY_HOUR,
      listen(actions.MeasurablesStateMachine, 'toResolving'));
    emitter.on(events.EVERY_MINUTE,
      listen(consumers.Emails));
    emitter.on(events.EVERY_MINUTE,
      listen(actions.UserUpdater));
    emitter.on(events.EVERY_HOUR,
      listen(actions.MaterializedViewsUpdater));
    emitter.on(events.MAIL,
      listen(Mailer));

    // MEASURABLE_CHANGED
    emitter.on(events.MEASURABLE_CHANGED,
      listen(producers.notifications.MeasurableStateChanged));
    emitter.on(events.MEASURABLE_CHANGED,
      listen(producers.notifications.MeasurableStateResolved));
    emitter.on(events.MEASURABLE_CHANGED,
      listen(producers.feedItems.NewMeasurableReachedResolution));
    // emitter.on(events.MEASURABLE_CHANGED,
    //   listenFor(actions.Notifications, 'updateMeasurableSlackNotification'));

    // NEW_MEMBERSHIP
    emitter.on(events.NEW_MEMBERSHIP,
      listen(producers.notifications.MemberAddedToCommunity));
    emitter.on(events.NEW_MEMBERSHIP,
      listen(producers.feedItems.MemberJoinedCommunity));

    // NEW_MEASUREMENT
    emitter.on(events.NEW_MEASUREMENT,
      listen(producers.feedItems.NewMeasurementPrediction));
    emitter.on(events.NEW_MEASUREMENT,
      listen(producers.feedItems.NewMeasurementComment));
    emitter.on(events.NEW_MEASUREMENT,
      listen(producers.feedItems.NewMeasurementResolution));
    emitter.on(events.NEW_MEASUREMENT,
      listen(producers.feedItems.NewMeasurementNotAvailable));
    // emitter.on(events.NEW_MEASUREMENT,
    //   listenFor(actions.Notifications, 'newMeasurementSlackNotification'));
    emitter.on(events.NEW_MEASUREMENT,
      listen(actions.MeasurablesStateMachine, 'measurableStateTransition'));

    //  NEW_MEASURABLE
    emitter.on(events.NEW_MEASURABLE,
      listen(producers.feedItems.NewMeasurable));
    // emitter.on(events.NEW_MEASURABLE,
    //   listenFor(actions.Notifications, 'newMeasurableSlackNotification'));

    // NEW_CHANNEL
    emitter.on(events.NEW_CHANNEL,
      listen(producers.feedItems.NewChannel));
    emitter.on(events.NEW_CHANNEL,
      listen(actions.Creators, 'createChannelMembership'));

    // Common.
    emitter.on(events.NEW_SERIES,
      listen(actions.Creators, 'createMeasurables'));
    emitter.on(events.SERIES_CHANGED,
      listen(actions.Creators, 'createNewMeasurables'));
    emitter.on(events.CREATING_BOT,
      listen(actions.Creators, 'createBotAgent'));
    emitter.on(events.CREATING_USER,
      listen(actions.Creators, 'createUserAgent'));
    emitter.on(events.UPDATING_MEASURABLE,
      listen(actions.Creators, 'checkMeasurableState'));
    emitter.on(events.VALIDATING_MEASUREMENT,
      listen(actions.Creators, 'checkMeasurement'));
    emitter.on(events.NEW_INVITATION,
      listen(producers.notifications.Invitation));
    emitter.on(events.NEW_USER,
      listen(actions.Invitations, 'transition'));
    emitter.on(events.USER_CHANGED,
      listen(actions.Invitations, 'transition'));
    emitter.on(events.USER_CHANGED,
      listen(actions.UserUpdater, 'updateUser'));

    emitter.on(events.NEW_NOTEBOOK,
      listen(producers.feedItems.NewNotebook));
  } catch (e) {
    console.error('Listener error', e);
  }
}

module.exports = {
  runListeners,
};
