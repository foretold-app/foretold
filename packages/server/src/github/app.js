const express = require('express');
const app = express();

app.get('/', (req, res, next) => {
  console.log('Get for Hooks App.');
  res.send('OK');
});

app.post('/', (req, res, next) => {
  console.log('Req.url', req.url);
  console.log('Req.body', req.body);
  res.send('OK');
});

module.exports = {
  app
};
