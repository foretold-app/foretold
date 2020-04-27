const bunyan = require('bunyan');
const PrettyStream = require('bunyan-prettystream');

const config = require('../../../config/log');

const logger = process.env.NODE_ENV === 'development'
  ? require('bunyan-buddy')({
    ...config,
    local_level: 0,
    remote_level: null,
  })
  : require('bunyan-buddy')(config);

if (process.env.NODE_ENV === 'development') {
  const prettyStdOut = new PrettyStream();
  prettyStdOut.pipe(process.stdout);

  const map = {
    trace: 10,
    debug: 20,
    info: 30,
    warn: 40,
    error: 50,
  };

  logger.streams.unshift({
    level: map[config.local_level] || 20,
    raw: true,
    stream: prettyStdOut,
  });
  logger._level = map[config.local_level] || 20;

  logger.module = (moduleName) => {
    return bunyan.createLogger({
      name: config.app.name,
      streams: logger.streams,
      module: moduleName,
    });
  };
}

module.exports = logger;
