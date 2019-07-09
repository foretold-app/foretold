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
  return datas.preferences.getCreateOne(params, query, data);
}

module.exports = {
  getOne,
};
