const emitter = require('../emitter');
const events = require('../events');

require('../listeners').runListeners();

emitter.emit(events.AGGREGATION_BOT);
