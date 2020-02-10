const _ = require('lodash');

const { ChannelBookmarksData } = require('../../data');

const { Options } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Data } = require('../../data/classes');
const { Query } = require('../../data/classes');

/**
 * @param {*} _root
 * @param {object} args
 * @param {Models.ChannelID} args.channelId
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.User>}
 */
async function create(_root, args, context, _info) {
  const channelId = _.get(args, 'channelId', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const data = new Data({
    channelId,
    agentId: currentAgentId,
  });

  return new ChannelBookmarksData().createOne(data);
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Models.ChannelID} args.channelId
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Model>}
 */
async function remove(_root, args, context, _info) {
  const channelId = _.get(args, 'channelId', null);

  const isAdmin = _.get(context, 'agent.isAdmin', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const params = new Params({ channelId });
  const query = new Query();
  const options = new Options({ isAdmin, currentAgentId });

  return new ChannelBookmarksData().deleteOne(params, query, options);
}

module.exports = {
  create,
  remove,
};
