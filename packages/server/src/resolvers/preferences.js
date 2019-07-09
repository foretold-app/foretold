const _ = require('lodash');

const datas = require('../data');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function getOne(root, args, context, info) {
  const agentId = _.get(root, 'id');
  const params = { agentId };
  const query = {};
  const data = { agentId };
  return datas.preferences.upsertOne(params, query, data);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.User>}
 */
async function update(root, args, context, info) {
  const params = { id: args.id };
  const data = args.input;
  const options = {};
  return datas.preferences.updateOne(params, data, options);
}

module.exports = {
  getOne,
  update,
};
