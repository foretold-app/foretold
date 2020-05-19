const _ = require('lodash');
const { rule } = require('graphql-shield');
const logger = require('../../lib/log');

const log = logger.module('authorizers/bots');

/**
 * @param {object} root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {boolean}
 */
function notebookIsOwnedByCurrentAgentRule(root, _args, context, _info) {
  const notebookAgentId = _.get(context, 'notebook.ownerId', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const result = (!!notebookAgentId && !!currentAgentId)
    && notebookAgentId === currentAgentId;

  log.trace(
    '\x1b[33m Rule Notebooks (notebookBelongsToCurrentAgentRule), '
    + `result = "${result}".\x1b[0m`,
  );

  return result;
}

/** @type {Rule} */
const notebookIsOwnedByCurrentAgent = rule({
  cache: 'no_cache',
})(notebookIsOwnedByCurrentAgentRule);

module.exports = {
  notebookIsOwnedByCurrentAgent,
};
