const express = require('express');
const app = express();

const { api } = require('./api');

app.get('/', (req, res, next) => {
  console.log('Get for Hooks App.');
  // res.send('OK');
  api.getListOfHooks().then((list) => {
    res.send(list);
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
