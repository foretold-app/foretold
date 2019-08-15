const express = require('express');
const bodyParser = require('body-parser');

const { Trigger } = require('./trigger');

const app = express();
app.use(bodyParser.json());

app.post('/', (req, res) => {
  console.log('GitHub incoming hook.');
  console.log('Req.url', req.url);

  const webhook = req.body;
  new Trigger(webhook).main().then((result) => {
    console.log(`GitHub trigger result`, result);
  }, (err) => {
    console.log(`GitHut trigger error`, err.message);
  });

  res.send('OK');
});

module.exports = {
  app
};
