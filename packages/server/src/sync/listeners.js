const actions = require('./actions');
const consumers = require('./consumers');
const { Mailer } = require('./mailer');
const { GitHubApi } = require('../lib/github/git-hub-api');
const logger = require('../lib/log');

const log = logger.module('sync/listeners');

/**
 * @returns {Promise<boolean>}
 */
async function updateUsers() {
  const name = 'Job::updateUsers';
  log.trace(name);

  try {
    const userUpdater = new actions.UserUpdater();
    const result = await userUpdater.main();
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}
/**
 * @param {Models.User} user
 * @returns {Promise<boolean>}
 */
async function updateUser(user) {
  const name = 'Job::updateUser';
  log.trace(name);

  try {
    const userUpdater = new actions.UserUpdater();
    const result = await userUpdater.updateUser(user);
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @param {Models.User} user
 * @returns {Promise<boolean>}
 */
async function invitations(user) {
  const name = 'Job::invitations';
  log.trace(name);

  try {
    const invitation$ = new actions.Invitations();
    const result = await invitation$.transition(user);
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @param {Models.Channel} channel
 * @returns {Promise<boolean>}
 */
async function createChannelMembership(channel) {
  const name = 'Job::createChannelMembership';
  log.trace(name);

  try {
    const creators = new actions.Creators();
    const result = await creators.createChannelMembership(channel);
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @param {Models.Bot} bot
 * @returns {Promise<boolean>}
 */
async function createBotAgent(bot) {
  const name = 'Job::createBotAgent';
  log.trace(name);

  try {
    const creators = new actions.Creators();
    const result = await creators.createBotAgent(bot);
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @param {Models.User} user
 * @returns {Promise<boolean>}
 */
async function createUserAgent(user) {
  const name = 'Job::createUserAgent';
  log.trace(name);

  try {
    const creators = new actions.Creators();
    const result = await creators.createUserAgent(user);
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @param {Models.Measurable} measurable
 * @returns {Promise<boolean>}
 */
async function checkMeasurable(measurable) {
  const name = 'Job::checkMeasurable';
  log.trace(name);

  try {
    const creators = new actions.Creators();
    const result = await creators.checkMeasurableState(measurable);
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @param {Models.Measurement} measurement
 * @returns {Promise<boolean>}
 */
async function checkMeasurement(measurement) {
  const name = 'Job::checkMeasurement';
  log.trace(name);

  try {
    const creators = new actions.Creators();
    const result = await creators.checkMeasurement(measurement);
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @param {Models.Measurement} measurement
 * @returns {Promise<boolean>}
 */
async function measurableStateTransition(measurement) {
  const name = 'Job::measurableStateTransition';
  log.trace(name);

  try {
    const action = new actions.MeasurablesStateMachine();
    const result = await action.measurableStateTransition(measurement);
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @param {Models.Series} series
 * @returns {Promise<boolean>}
 */
async function createNewMeasurables(series) {
  const name = 'Job::createNewMeasurables';
  log.trace(name);

  try {
    const creators = new actions.Creators();
    const result = await creators.createMeasurables(series);
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @param {Models.Measurement} measurement
 * @returns {Promise<boolean>}
 */
async function newMeasurementSlackNotification(measurement) {
  const name = 'Job::newMeasurementSlackNotification';
  log.trace(name);

  try {
    const notifications = new actions.Notifications();
    const result = await notifications.newMeasurementSlackNotification(
      measurement,
    );
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @param {Models.Measurable} measurable
 * @returns {Promise<boolean>}
 */
async function newMeasurableSlackNotification(measurable) {
  const name = 'Job::newMeasurableSlackNotification';
  log.trace(name);

  try {
    const notifications = new actions.Notifications();
    const result = await notifications.newMeasurableSlackNotification(
      measurable,
    );
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @param {Models.Measurable} measurable
 * @returns {Promise<boolean>}
 */
async function updateMeasurableSlackNotification(measurable) {
  const name = 'Job::updateMeasurableSlackNotification';
  log.trace(name);

  try {
    const notifications = new actions.Notifications();
    const result = await notifications.updateMeasurableSlackNotification(
      measurable,
    );
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @returns {Promise<boolean>}
 */
async function toJudgementPendingTransition() {
  const name = 'Job::toJudgementPendingTransition';
  log.trace(name);

  try {
    const reducer = new actions.MeasurablesStateMachine();
    const result = await reducer.toJudgementPending();
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @returns {Promise<boolean>}
 */
async function toResolving() {
  const name = 'Job::toResolving';
  log.trace(name);

  try {
    const reducer = new actions.MeasurablesStateMachine();
    const result = await reducer.toResolving();
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @returns {Promise<boolean>}
 */
async function emailConsumer() {
  const name = '\x1b[35mJob::emailConsumer\x1b[0m';
  log.trace(name);

  try {
    const consumer = new consumers.Emails();
    const result = await consumer.main();
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @param {Object} envelop
 * @returns {Promise<boolean>}
 */
async function mailer(envelop = {}) {
  const name = '\x1b[35mJob::mailer\x1b[0m';
  log.trace(name);

  try {
    const result = await new Mailer(envelop).main();
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

/**
 * @param {function} Producer
 * @returns {function(*=): boolean}
 */
function listenFor(Producer) {
  const name = `${Producer.name}`;
  log.trace(`Listen for: ${name}`);

  return async function listenForProducer(input) {
    log.trace(`Run listener: ${name}`);

    try {
      const producer = new Producer(input);
      const result = await producer.main();
      log.trace(name, 'all done', result);
    } catch (e) {
      console.error(name, e.message, e);
    }

    return true;
  };
}

/**
 * @returns {Promise<boolean>}
 */
async function addGitHubWebHook() {
  const name = 'Job::addGitHubWebHook';
  log.trace(name);

  try {
    const gitHubApi = new GitHubApi();
    const result = await gitHubApi.addHook();
    log.trace(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

module.exports = {
  toJudgementPendingTransition,
  toResolving,
  emailConsumer,
  mailer,
  listenFor,
  addGitHubWebHook,
  measurableStateTransition,

  newMeasurementSlackNotification,
  newMeasurableSlackNotification,
  updateMeasurableSlackNotification,

  createNewMeasurables,
  createChannelMembership,
  createBotAgent,
  createUserAgent,
  checkMeasurable,
  checkMeasurement,

  invitations,
  updateUser,
  updateUsers,
};
