const path = require('path');
const cors = require('cors');
const express = require('express');
const bodyParser = require('body-parser-graphql');

const config = require('../config/config');
const { runJobs, runListeners } = require('./sync');
const { events, emitter } = require('./sync');
const { apolloServer } = require('./graphql/apollo-server');
const logger = require('./lib/log');

const log = logger.module('src/index.js');

{
  // Makes sync flows possible
  runJobs();
  runListeners();
}

const app = express();
app.use(cors());

{
  /**
   * Always remember, that services like "Heroku.com"
   * use proxies.
   */
  const redirectRequestToHttps = (req, res, next) => {
    const isProtocolHttps = req.protocol === 'https';
    const isSecure = req.secure === true;
    const isHeaderHttps = req.headers['x-forwarded-proto'] === 'https';
    const isConnectionEnc = !!(req.connection && req.connection.encrypted);

    const secure = isProtocolHttps
      || isSecure
      || isHeaderHttps
      || isConnectionEnc;

    if (secure) {
      next();
    } else {
      // @todo: fix it later
      res.redirect('https://' + 'www.foretold.io' + req.url);
    }
  };

  if (config.PROD) {
    app.use(redirectRequestToHttps);
  }
}

{
  // Returns the client's files
  const fallbackFile = path.resolve(__dirname, '../../client/dist/index.html');
  const distDir = path.resolve(__dirname, '../../client/dist');

  log.info('Fallback file', fallbackFile);
  log.info('Dist dir', distDir);

  // Returns all routes excluding "/graphql", "/hooks", "/env" as static files
  // or returns fallback page.
  app.get(/^((?!(graphql|hooks|env)).)*$/,
    express.static(distDir),
    (req, res) => res.sendFile(fallbackFile));
}

{
  // Do not set API_URL in "Heroku.com" for Staging
  // (this env is used for PR building too).
  // Do not set API_URL in "Heroku.com" for Production.
  app.get(/^\/env\.([0-9a-z]+)\.js$/, (_req, res) => res.send(
    'window.ENV = { '
    + `API_URL: "${config.API_URL}", `
    + `AUTH0_DOMAIN: "${config.AUTH0_DOMAIN}", `
    + `AUTH0_CLIENT_ID: "${config.AUTH0_CLIENT_ID}", `
    + `CLIENT_ENV: "${process.env.NODE_ENV}", `
    + `INTERCOM_APP_ID: "${config.INTERCOM_APP_ID}", `
    + '};',
  ));
}

{
  // Supporting of GitHub integration
  const { app: subApp } = require('./sync/github/app');
  app.use('/hooks', subApp);
}

{
  // Supporting of Graphql server
  app.use(bodyParser.graphql());
  apolloServer.applyMiddleware({ app });
}

app.listen({ port: config.PORT }, () => {
  log.info(`Server ready at http://localhost:${config.PORT}`);
  emitter.emit(events.SERVER_IS_READY, app);
});
