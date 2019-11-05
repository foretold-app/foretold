const _ = require('lodash');

const { NotebooksData } = require('../../data');

const { Pagination } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Filter } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Data } = require('../../data/classes');

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
 * @param {*} _root
 * @param {object} args
 * @param {Models.AgentID} args.ownerId
 * @param {Models.ChannelID} args.channelId
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function all(_root, args, _context, _info) {
  /** @type {Models.AgentID} */
  const ownerId = _.get(args, 'ownerId', null);
  /** @type {Models.ChannelID} */
  const channelId = _.get(args, 'channelId', null);

  const filter = new Filter({ ownerId, channelId });
  const pagination = new Pagination(args);
  const options = new Options();

  return new NotebooksData().getConnection(filter, pagination, options);
}


/**
 * @param {*} _root
 * @param {object} args
 * @param {Models.NotebookID} args.id
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<Model>}
 */
async function one(_root, args, _context, _info) {
  const id = _.get(args, 'id', null);
  const params = new Params({ id });
  return new NotebooksData().getOne(params);
}

module.exports = {
  all,
  one,
  create,
};
