const _ = require('lodash');

const { PreferencesData } = require('../../data');
const logger = require('../../lib/log');

const log = logger.module('middlewares/preferences');

/**
 * @param {object | null} _root
 * @param {object} args
 * @param {object} args.id
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextPreferenceFromId(_root, args, context, _info) {
  const id = _.get(args, 'id', null);

  log.trace(
    '\x1b[36m ---> \x1b[0m Middleware (setContextPreferenceFromId)',
    { id },
  );

  context.preference = !!id
    ? await new PreferencesData().getOne({ id })
    : null;
}

/**
 * @param {object | null} _root
 * @param {object} args
 * @param {object} args.id
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextPreferenceFromAgentId(_root, args, context, _info) {
  const agentId = _.get(args, 'agentId', null);

  log.trace(
    '\x1b[36m ---> \x1b[0m Middleware (setContextPreferenceFromAgentId)',
    { agentId },
  );

  context.preference = !!agentId
    ? await new PreferencesData().getOne({ agentId })
    : null;
}

module.exports = {
  setContextPreferenceFromId,
  setContextPreferenceFromAgentId,
};
