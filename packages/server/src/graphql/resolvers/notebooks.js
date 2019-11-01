const _ = require('lodash');

const { NotebooksData } = require('../../data');

const { Pagination } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Filter } = require('../../data/classes');

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
};
