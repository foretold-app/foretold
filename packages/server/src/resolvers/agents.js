const _ = require('lodash');

const data = require('../data');

const { Params } = require('../data/classes/params');
const { Filter } = require('../data/classes/filter');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Agent>}
 */
async function one(root, args, context, info) {
  const id = _.get(args, 'id')
    || _.get(root, 'agentId');
  const params = new Params({ id });
  return data.agents.getOne(params);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.excludeChannelId
 * @param {string[]} args.types
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function all(root, args, context, info) {
  const filter = new Filter({
    excludeChannelId: _.get(args, 'excludeChannelId'),
    types: _.get(args, 'types'),
  });
  return data.agents.getAll(filter);
}

module.exports = {
  one,
  all,
};
