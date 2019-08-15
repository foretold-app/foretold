const _ = require('lodash');
const express = require('express');
const bodyParser = require('body-parser');

const { Trigger } = require('./trigger');

const app = express();
app.use(bodyParser.json());

app.post('/', (req, res) => {
  console.log('GitHub incoming hook.');
  console.log('Req.url', req.url);

  const xHubSignature = req.header('X-Hub-Signature');
  if (!xHubSignature) {
    console.warn('X-Hub-Signature is empty');
    res.send('ERR');
    return;
  }

  const webhook = req.body;
  new Trigger(webhook, xHubSignature).main().then((result) => {
    console.log(`GitHub trigger result`, result);
  }, (err) => {
    console.warn(`GitHut trigger error`, err.message);
  });

  res.send('OK');
});

module.exports = {
  app
};
