const _ = require('lodash');

const { NotebooksData } = require('../../data');

const { Pagination } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Filter } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Data } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { structures } = require('../../data/classes');

const { HOME_CHANNEL_ID } = require('../../../config/well-known');

/**
 * @param {*} _root
 * @param {object} args
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.User>}
 */
async function create(_root, args, context, _info) {
  const input = _.get(args, 'input') || {};
  const ownerId = _.get(context, 'agent.id', null);

  const data = new Data({
    ...input,
    ownerId,
  });

  return new NotebooksData().createOne(data);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Defs.NotebookID} args.id
 * @param {object} args.input
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function update(root, args, _context, _info) {
  const id = _.get(args, 'id', null);
  const input = _.get(args, 'input') || {};
  const params = new Params({ id });
  return new NotebooksData().updateOne(params, input);
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Defs.AgentID} args.ownerId
 * @param {Defs.ChannelID} args.channelId
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function all(_root, args, context, _info) {
  /** @type {Defs.AgentID | null} */
  const ownerId = _.get(args, 'ownerId', null);
  /** @type {Defs.ChannelID | null} */
  const channelId = _.get(args, 'channelId', null);

  /** @type {boolean | null} */
  const isAdmin = _.get(context, 'agent.isAdmin', null);
  /** @type {Defs.AgentID | null} */
  const currentAgentId = _.get(context, 'agent.id', null);

  /**
   * @todo: search tag: "withinJoinedChannelsByChannelId"
   * @type {Layers.withinJoinedChannels | null}
   */
  const withinJoinedChannels = channelId === HOME_CHANNEL_ID && !!currentAgentId
    ? structures.withinJoinedChannelsByChannelId(currentAgentId) : null;

  const filter = new Filter({ ownerId, channelId, withinJoinedChannels });
  const pagination = new Pagination(args, {
    agentId: currentAgentId,
  });
  const options = new Options({
    isAdmin,
    currentAgentId,
    raw: true,
    attributes: {
      bookmarksCount: true,
      isBookmarked: { agentId: currentAgentId },
    },
  });

  return new NotebooksData().getConnection(filter, pagination, options);
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Defs.NotebookID} args.id
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Model>}
 */
async function one(_root, args, context, _info) {
  const notebookId = _.get(args, 'id', null);

  const isAdmin = _.get(context, 'agent.isAdmin', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const params = new Params({ id: notebookId });
  const query = new Query();
  const options = new Options({
    isAdmin,
    currentAgentId,
    raw: true,
    attributes: {
      bookmarksCount: true,
      isBookmarked: { agentId: currentAgentId },
    },
  });

  return new NotebooksData().getOne(params, query, options);
}

/**
 * @param {*} root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function count(root, _args, _context, _info) {
  const channelId = _.get(root, 'id', null);
  const params = new Params({ channelId });
  return new NotebooksData().getCount(params);
}


/**
 * @param {*} _root
 * @param {object} args
 * @param {Defs.NotebookID} args.id
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Model>}
 */
async function remove(_root, args, context, _info) {
  const notebookId = _.get(args, 'id', null);

  const isAdmin = _.get(context, 'agent.isAdmin', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const params = new Params({ id: notebookId });
  const query = new Query();
  const options = new Options({ isAdmin, currentAgentId });

  return new NotebooksData().deleteOne(params, query, options);
}

module.exports = {
  all,
  one,
  create,
  update,
  count,
  remove,
};
