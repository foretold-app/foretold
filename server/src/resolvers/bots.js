const _ = require('lodash');
const data = require('../data');

/**
 * @param {*} root
 * @param {object} args
 * @param {string} args.id
 * @param {object} args.input
 * @param {Schema.Context} options
 * @param {object} info
 * @returns {Promise<Models.User>}
 */
async function create(root, args, options, info) {
  const datas = {
    ...args.input,
    userId: _.get(options, 'user.id'),
  };
  return await data.bots.createOne(datas);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function all(root, args, context, info) {
  const userId = _.get(context, 'user.id');
  return await data.bots.getAll({ ...args, userId });
}


module.exports = {
  all,
  create,
};
