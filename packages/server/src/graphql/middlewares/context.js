const _ = require('lodash');

const { authentication } = require('../authentication');
const { GlobalSettingsData } = require('../../data');
const logger = require('../../lib/log');

const log = logger.module('middlewares/init-context');

/**
 * @param req
 */
async function context({ req }) {
  const identity = await authentication(req);
  const ip = req.ip;

  const gs = new GlobalSettingsData();
  const globalSettings = await gs.getMain();
  const botAgentId = await gs.getBotAgentId(globalSettings);

  const initContext$ = {
    ...identity,
    ip,
    botAgentId,
    globalSettings,
  };

  const logs = {
    'User Id': _.get(initContext$, 'user.id', null),
    'Agent Id': _.get(initContext$, 'agent.id', null),
    'Bot Id': _.get(initContext$, 'bot.id', null),
    'Bot User Owner Id': _.get(initContext$, 'botUserOwner.id', null),
    'Creator Id': _.get(initContext$, 'creator.id', null),
    'Creator Name':
      _.get(initContext$, 'creator.constructor.name', null),
    'Settings Bot Agent Id': _.get(initContext$, 'botAgentId', null),
    IP: _.get(initContext$, 'ip'),
  };

  log.trace(' ✓ Context', JSON.stringify(logs));

  return initContext$;
}

module.exports = {
  context,
};
