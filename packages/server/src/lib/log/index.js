const bunyan = require('bunyan');
const PrettyStream = require('bunyan-prettystream');
const bunyanBuddy = require('bunyan-buddy');

const config = require('../../../config/log');

const map = {
  trace: 10,
  debug: 20,
  info: 30,
  warn: 40,
  error: 50,
};

const logger = bunyanBuddy(config);

if (process.env.NODE_ENV === 'development') {
  const prettyStdOut = new PrettyStream();
  prettyStdOut.pipe(process.stdout);

  const level = map[config.local_level] || map.debug;

  logger.streams = [{
    level,
    raw: true,
    stream: prettyStdOut,
  }];
  logger._level = level;

  logger.module = (moduleName) => {
    return bunyan.createLogger({
      name: config.app.name,
      streams: logger.streams,
      module: moduleName,
    });
  };
}


module.exports = logger;
