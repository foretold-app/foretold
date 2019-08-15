const express = require('express');
const bodyParser = require('body-parser');

const { api } = require('./api');
const { Trigger } = require('./trigger');

const app = express();
app.use(bodyParser.json());

app.get('/list', (req, res) => {
  console.log('Get for Hooks App.');

  api.getListOfHooks().then((list) => {
    res.send(list);
  }).catch((err) => {
    console.error(err);
    res.send('ERR');
  })
});

app.get('/files', (req, res) => {
  console.log('Get for Hooks App.');

  api.getPullFiles().then((list) => {
    res.send(list);
  }).catch((err) => {
    console.error(err);
    res.send('ERR');
  })
});

app.get('/file', (req, res) => {
  console.log('Get for Hooks App.');

  api.getDataJson().then((result) => {
    res.send(result);
  }).catch((err) => {
    console.error(err);
    res.send('ERR');
  })
});

app.get('/add', (req, res) => {
  console.log('Add Hook');

  api.addHook().then((result) => {
    res.send(result);
  }).catch((err) => {
    console.error(err);
    res.send('ERR');
  })
});

app.post('/', (req, res, next) => {
  console.log('GitHub incoming hook.');
  console.log('Req.url', req.url);
  console.log('Req.body', req.body);
  console.log('Req.body', JSON.stringify(req.body));

  const webhook = req.body;
  new Trigger(webhook).then((result) => {
    console.log(`GitHub trigger result`, result);
  }, (err) => {
    console.log(`GitHut trigger error`, err.message);
  });

  res.send('OK');
});

module.exports = {
  app
};
