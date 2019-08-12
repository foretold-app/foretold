const emitter = require('./emitter');
const events = require('./events');

const bots = require('../bots');

function runAggregationBot() {
  try {
    console.log('Job::runAggregationBot');
    new bots.Bot().main().then((result) => {
      console.log('Job::runAggregationBot', 'all done', result);
    }).catch((err) => {
      console.error('Job::runAggregationBot', err);
      console.error(err);
    });
  } catch (e) {
    console.log('Job::runAggregationBot error');
    console.error(e);
  }
}

function runListeners() {
  try {
    console.log('Jobs are ready.');
    emitter.on(events.AGGREGATION_BOT, runAggregationBot);
  } catch (e) {
    console.log('Listener error');
    console.error(e);
  }
}

module.exports = {
  runListeners,
};
