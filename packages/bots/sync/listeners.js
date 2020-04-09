const emitter = require('./emitter');
const events = require('./events');

const logger = require('../log');

const log = logger.module('bots/listeners');
const bots = require('../bots');

function runAggregationBot() {
  try {
    log.trace('Job::runAggregationBot');
    new bots.Bot().main().then((result) => {
      log.trace('Job::runAggregationBot', 'all done', result);
    }).catch((err) => {
      log.error('Job::runAggregationBot', err);
      log.error(err);
    });
  } catch (e) {
    log.trace('Job::runAggregationBot error');
    log.error(e);
  }
}

function runListeners() {
  try {
    log.trace('Jobs are ready.');
    emitter.on(events.AGGREGATION_BOT, runAggregationBot);
  } catch (e) {
    log.trace('Listener error');
    log.error(e);
  }
}

module.exports = {
  runListeners,
};
