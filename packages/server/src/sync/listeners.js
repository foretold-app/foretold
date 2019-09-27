const actions = require('./actions');
const consumers = require('./consumers');
const { Mailer } = require('./mailer');
const { GitHubApi } = require('../lib/github/git-hub-api');

async function measurableStateTransition(measurement) {
  const name = 'Job::measurableStateTransition';
  console.log(name);

  try {
    const action = new actions.MeasurablesStateMachine();
    const result = await action.measurableStateTransition(measurement);
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

async function createNewMeasurables(series) {
  const name = 'Job::createNewMeasurables';
  console.log(name);

  try {
    const creators = new actions.Creators();
    const result = await creators.createMeasurables(series);
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

async function newMeasurement(measurement) {
  const name = 'Job::newMeasurement';
  console.log(name);

  try {
    const notifications = new actions.Notifications();
    const result = await notifications.newMeasurement(measurement);
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

async function newMeasurable(measurable) {
  const name = 'Job::newMeasurable';
  console.log(name);

  try {
    const notifications = new actions.Notifications();
    const result = await notifications.newMeasurable(measurable);
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

async function updateMeasurable(measurable) {
  const name = 'Job::updateMeasurable';
  console.log(name);

  try {
    const notifications = new actions.Notifications();
    const result = await notifications.updateMeasurable(measurable);
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

async function toJudgementPendingTransition() {
  const name = 'Job::toJudgementPendingTransition';
  console.log(name);

  try {
    const reducer = new actions.MeasurablesStateMachine();
    const result = await reducer.toJudgementPending();
    console.log(name, 'all done', result);
  } catch (e) {
    console.error(name, e.message, e);
  }

  return true;
}

async function toResolving() {
  const name = 'Job::toResolving';
  console.log(name);

  try {
    const reducer = new actions.MeasurablesStateMachine();
    const result = await reducer.toResolving();
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

module.exports = {
  toJudgementPendingTransition,
  toResolving,
  emailConsumer,
  mailer,
  listenFor,
  addGitHubWebHook,
  newMeasurement,
  newMeasurable,
  updateMeasurable,
  createNewMeasurables,
  measurableStateTransition,
};
