const _ = require('lodash');

const data = require('../data');

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<boolean>}
 */
async function all(root, args, context, info) {
  return data.feedItemsData.getAll();
}

module.exports = {
  all,
};
