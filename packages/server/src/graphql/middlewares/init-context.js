const _ = require('lodash');

const { authentication } = require('../authentication');
const { GlobalSettingsData } = require('../../data');
const logger = require('../../lib/log');

const log = logger.module('middlewares/init-context');

/**
 *
 * @param req
 */
async function initContext({ req }) {
  const identity = await authentication(req);
  const botAgentId = await new GlobalSettingsData().getBotAgentId();
  const ip = req.ip;

  const initContext$ = {
    ...identity,
    botAgentId,
    ip,
  };

  const logs = {
    'Identity User Id': _.get(initContext$, 'user.id', null),
    'Identity Agent Id': _.get(initContext$, 'agent.id', null),
    'Identity Bot Id': _.get(initContext$, 'bot.id', null),
    'Identity Creator Id': _.get(initContext$, 'creator.id', null),
    'Identity Creator Name':
      _.get(initContext$, 'creator.constructor.name', null),
    'Settings Bot Agent Id': _.get(initContext$, 'botAgentId', null),
    IP: _.get(initContext$, 'ip'),
  };

  log.trace(' ✓ Context', JSON.stringify(logs));

  return initContext$;
}

module.exports = {
  initContext,
};
