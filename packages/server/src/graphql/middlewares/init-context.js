const _ = require('lodash');

const { authentication } = require('../../authentication');
const { globalSettings } = require('../../data');

/**
 *
 * @param req
 */
async function initContext({ req }) {
  const identity = await authentication(req);
  const botAgentId = await globalSettings.getBotAgentId();

  const initContext = { ...identity, botAgentId };

  console.log(' --- ');
  console.log(' ✓ Context Identity User Id', _.get(initContext, 'user.id'));
  console.log(' ✓ Context Identity Agent Id', _.get(initContext, 'agent.id'));
  console.log(' ✓ Context Identity Bot Id', _.get(initContext, 'bot.id'));
  console.log(' ✓ Context Identity Creator Id', _.get(initContext, 'creator.id'));
  console.log(' ✓ Context Identity Creator Name', _.get(initContext, 'creator.constructor.name'));
  console.log(' ✓ Context Settings Bot Agent Id', _.get(initContext, 'botAgentId'));
  console.log(' --- ');

  return initContext;
}

module.exports = {
  initContext,
};
