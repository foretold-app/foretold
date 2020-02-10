const _ = require('lodash');

const { BookmarksData } = require('../../data');

const { Options } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Data } = require('../../data/classes');
const { Query } = require('../../data/classes');

/**
 * @param {*} _root
 * @param {object} args
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.User>}
 */
async function create(_root, args, context, _info) {
  const input = _.get(args, 'input', null) || {};
  const currentAgentId = _.get(context, 'agent.id', null);

  const data = new Data({
    ...input,
    agentId: currentAgentId,
  });

  return new BookmarksData().createOne(data);
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Models.BookmarkID} args.bookmarkId
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Model>}
 */
async function remove(_root, args, context, _info) {
  const bookmarkId = _.get(args, 'bookmarkId', null);

  const isAdmin = _.get(context, 'agent.isAdmin', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const params = new Params({ id: bookmarkId });
  const query = new Query();
  const options = new Options({ isAdmin, currentAgentId });

  return new BookmarksData().deleteOne(params, query, options);
}

module.exports = {
  create,
  remove,
};
