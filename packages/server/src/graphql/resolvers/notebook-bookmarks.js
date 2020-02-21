const _ = require('lodash');

const { NotebookBookmarksData } = require('../../data');

/**
 * @param {*} _root
 * @param {object} args
 * @param {Definitions.ChannelID} args.channelId
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<boolean>}
 */
async function toggle(_root, args, context, _info) {
  const notebookId = _.get(args, 'notebookId', null);
  const currentAgentId = _.get(context, 'agent.id', null);
  return new NotebookBookmarksData().toggle(notebookId, currentAgentId);
}

module.exports = {
  toggle,
};
