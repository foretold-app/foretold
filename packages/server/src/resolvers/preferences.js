const _ = require('lodash');

const data = require('../data');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function getOne(root, args, context, info) {
  const agentId = _.get(root, 'agentId');
  const params = { agentId };
  return data.preferences.getCreateOne(params);
}

module.exports = {
  getOne,
};
