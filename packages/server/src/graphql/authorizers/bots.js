const _ = require('lodash');
const { rule } = require('graphql-shield');
const logger = require('../../lib/log');

const log = logger.module('authorizers/bots');

/**
 * @todo: To fix "||" a joined logic.
 * @param {object} root
 * @param {string} root.userId
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {boolean}
 */
function botBelongsToCurrentUserRule(root, args, context, _info) {
  const botUserId = _.get(root, 'userId', null)
    || _.get(context, 'bot.userId', null);
  const userId = _.get(context, 'user.id', null);
  const result = !!botUserId && botUserId === userId;

  log.trace(
    '\x1b[33m Rule Bots (botBelongsToCurrentUser), '
    + `result = "${result}".\x1b[0m`,
  );

  return result;
}

/** @type {Rule} */
const botBelongsToCurrentUser = rule({
  cache: 'no_cache',
})(botBelongsToCurrentUserRule);

module.exports = {
  botBelongsToCurrentUser,
};
