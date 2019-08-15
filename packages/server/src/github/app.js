const express = require('express');
const app = express();

const { api } = require('./api');

app.get('/list', (req, res) => {
  console.log('Get for Hooks App.');

  api.getListOfHooks().then((list) => {
    res.send(list);
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
  console.log('Req.url', req.url);
  console.log('Req.body', req.body);
  res.send('OK');
});

module.exports = {
  app
};
