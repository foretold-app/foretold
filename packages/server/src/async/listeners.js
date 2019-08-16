const emitter = require('./emitter');
const events = require('./events');

const measurables = require('./measurables');
const producers = require('./producers');
const consumers = require('./consumers');
const { Mailer } = require('./mailer');
const { GitHubApi } = require('../github/git-hub-api');

/**
 * @todo: To avoid code duplicates.
 */

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

async function emailConsumer() {
  const name = '\x1b[35mJob::emailConsumer\x1b[0m';
  console.log(name);

  try {
    const consumer = new consumers.Emails();
    const result = await consumer.main();
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

async function mailer(envelop = {}) {
  const name = '\x1b[35mJob::mailer\x1b[0m';
  console.log(name);

  try {
    const result = await new Mailer(envelop).main();
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

function listenFor(Producer) {
  const name = `${Producer.name}`;
  console.log(`Listen for: ${name}`);

  return async function (input) {
    console.log(`Run listener: ${name}`);

    try {
      const producer = new Producer(input);
      const result = await producer.main();
      console.log(name, 'all done', result);
    } catch (e) {
      console.error(name, e.message, e);
    }

    return true;
  };
}

async function addGitHubWebHook() {
  const name = 'Job::addGitHubWebHook';
  console.log(name);

  try {
    const gitHubApi = new GitHubApi();
    const result = await gitHubApi.addHook();
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

function runListeners() {
  console.log('Listeners are in a queue.');
  try {
    emitter.on(events.SERVER_IS_READY, addGitHubWebHook);

    emitter.on(events.EVERY_HOUR, toJudgementPendingTransition);
    emitter.on(events.EVERY_MINUTE, emailConsumer);
    emitter.on(events.MAIL, mailer);

    emitter.on(events.MEASURABLE_CHANGED, listenFor(producers.notifications.MeasurableStateChanged));
    emitter.on(events.MEASURABLE_CHANGED, listenFor(producers.notifications.MeasurableStateResolved));
    emitter.on(events.MEASURABLE_CHANGED, listenFor(producers.feedItems.NewMeasurableReachedResolution));

    emitter.on(events.NEW_MEMBERSHIP, listenFor(producers.notifications.MemberAddedToCommunity));
    emitter.on(events.NEW_MEMBERSHIP, listenFor(producers.notifications.MemberInvitedToCommunity));
    emitter.on(events.NEW_MEMBERSHIP, listenFor(producers.feedItems.MemberJoinedCommunity));

    emitter.on(events.NEW_MEASUREMENT, listenFor(producers.feedItems.NewMeasurementPrediction));
    emitter.on(events.NEW_MEASUREMENT, listenFor(producers.feedItems.NewMeasurementComment));
    emitter.on(events.NEW_MEASUREMENT, listenFor(producers.feedItems.NewMeasurementResolution));
    emitter.on(events.NEW_MEASUREMENT, listenFor(producers.feedItems.NewMeasurementNotAvailable));

    emitter.on(events.NEW_MEASURABLE, listenFor(producers.feedItems.NewMeasurable));

  } catch (e) {
    console.error('Listener error', e);
  }
}

module.exports = {
  runListeners,
};
