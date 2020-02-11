const _ = require('lodash');

const { PreferencesData } = require('../../data');
const logger = require('../../lib/log');

const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Options } = require('../../data/classes');

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
  const preferenceId = _.get(args, 'id', null);

  log.trace(
    '\x1b[36m ---> \x1b[0m Middleware (setContextPreferenceFromId)',
    { preferenceId },
  );

  const params = new Params({ id: preferenceId });
  const query = new Query();
  const options = new Options({ raw: true });

  context.preference = !!preferenceId
    ? await new PreferencesData().getOne(params, query, options)
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

  const params = new Params({ agentId });
  const query = new Query();
  const options = new Options({ raw: true });

  context.preference = !!agentId
    ? await new PreferencesData().getOne(params, query, options)
    : null;
}

module.exports = {
  setContextPreferenceFromId,
  setContextPreferenceFromAgentId,
};
