const express = require('express');
const bodyParser = require('body-parser');

const logger = require('../../lib/log');
const { Trigger } = require('./trigger');

const log = logger.module('github/app');

const app = express();
app.use(bodyParser.json());

/**
 * This route catches incoming "WebHook" from
 * GitHub.com.
 */
app.post('/', (req, res) => {
  log.trace('GitHub incoming hook on', req.url);

  const xHubSignature = req.header('X-Hub-Signature');
  if (!xHubSignature) {
    log.warn('X-Hub-Signature is empty');
    res.send('ERR');
    return;
  }

  const webhook = req.body;
  new Trigger(webhook, xHubSignature).main().then((result) => {
    log.trace('GitHub trigger result', result);
  }, (err) => {
    log.warn('GitHut trigger error', err.message);
  });

  res.send('OK');
});

module.exports = {
  app,
};
