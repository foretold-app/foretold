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
    // required
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
  const datas = { ...args };
  return await data.bots.getAll(datas);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function one(root, args, context, info) {
  const id = _.get(args, 'id');
  return await data.bots.getOne({ id });
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function tokenRefresh(root, args, context, info) {
  const id = _.get(args, 'id');
  const jwt = await data.bots.tokenRefresh({ id });
  return { jwt };
}

module.exports = {
  all,
  one,
  create,
  tokenRefresh,
};
