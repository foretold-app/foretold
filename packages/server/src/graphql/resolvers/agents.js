const _ = require('lodash');

const data = require('../../data');

const { Params } = require('../../data/classes');
const { Filter } = require('../../data/classes');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<Models.Agent>}
 */
async function one(root, args, _context, _info) {
  const id = _.get(args, 'id', null)
    || _.get(root, 'agentId', null);
  const params = new Params({ id });
  return data.agents.getOne(params);
}

/**
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
  return data.agents.getAll(filter);
}

module.exports = {
  one,
  all,
};
