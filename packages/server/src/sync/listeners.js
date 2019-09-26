const actions = require('./actions');
const consumers = require('./consumers');
const { Mailer } = require('./mailer');
const { GitHubApi } = require('../lib/github/git-hub-api');

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
};
