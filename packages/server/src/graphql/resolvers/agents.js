const _ = require('lodash');

const { AgentsData } = require('../../data');

const { Params } = require('../../data/classes');
const { Filter } = require('../../data/classes');
const { Pagination } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Query } = require('../../data/classes');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.AgentID} args.id
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<Models.Agent>}
 */
async function one(root, args, _context, _info) {
  const agentId = _.get(args, 'id', null)
    || _.get(root, 'agentId', null)
    || _.get(root, 'creatorId', null)
    || _.get(root, 'ownerId', null);
  const params = new Params({ id: agentId });
  const query = new Query();
  const options = new Options({ raw: true, attributes: true });
  return new AgentsData().getOne(params, query, options);
}

/**
 * @todo: To use "getConnection" to limit queries.
 * @param {*} root
 * @param {object} args
 * @param {Models.ChannelID} args.excludeChannelId
 * @param {string[]} args.types
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function all(root, args, _context, _info) {
  const excludeChannelId = _.get(args, 'excludeChannelId', null);
  const types = _.get(args, 'types', null);

  const filter = new Filter({
    excludeChannelId,
    types,
  });
  const pagination = new Pagination();
  const options = new Options({ raw: true, attributes: true });

  return new AgentsData().getAll(filter, pagination, options);
}

/**
 * @param {object} _root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function count(_root, _args, _context, _info) {
  return new AgentsData().getCount();
}

module.exports = {
  all,
  count,
  one,
};
