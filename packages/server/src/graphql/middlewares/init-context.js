const _ = require('lodash');

const { authentication } = require('../authentication');
const { globalSettings } = require('../../data');
const logger = require('../../lib/log');

const log = logger.module('middlewares/init-context');

/**
 *
 * @param req
 */
async function initContext({ req }) {
  const identity = await authentication(req);
  const botAgentId = await globalSettings.getBotAgentId();
  const ip = req.ip;

  const initContext = {
    ...identity,
    botAgentId,
    ip,
  };

  log.trace(' --- ');
  log.trace(' ✓ Context Identity User Id',
    _.get(initContext, 'user.id'));
  log.trace(' ✓ Context Identity Agent Id',
    _.get(initContext, 'agent.id'));
  log.trace(' ✓ Context Identity Bot Id',
    _.get(initContext, 'bot.id'));
  log.trace(' ✓ Context Identity Creator Id',
    _.get(initContext, 'creator.id'));
  log.trace(' ✓ Context Identity Creator Name',
    _.get(initContext, 'creator.constructor.name'));
  log.trace(' ✓ Context Settings Bot Agent Id',
    _.get(initContext, 'botAgentId'))
  log.trace(' ✓ Context IP',
    _.get(initContext, 'ip'));
  log.trace(' --- ');

  return initContext;
}

module.exports = {
  initContext,
};
