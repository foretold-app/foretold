const _ = require('lodash');
const data = require('../data');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Agent>}
 */
async function one(root, args, context, info) {
  const id = _.get(args, 'id') || _.get(root, 'agentId');
  return await data.agents.getOne(id);
}

module.exports = {
  one,
};
