const _ = require('lodash');

const data = require('../data');
const { Pagination } = require('../pagination');

/**
 * @param {*} root
 * @param {object} args
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.User>}
 */
async function create(root, args, context, info) {
  const datas = {
    ...args.input,
    userId: _.get(context, 'user.id'),
  };
  return data.bots.createOne(datas);
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
  return data.bots.updateOne({ id: args.id }, args.input);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.ownerId
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Model[]>}
 */
async function all(root, args, context, info) {
  const filter = { userId: _.get(args, 'ownerId') };
  const pagination = new Pagination(args);
  const options = {};
  const result = await data.bots.getAll(filter, pagination, options);
  return result.data;
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
  return data.bots.getOne({ id });
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<{token: string}>}
 */
async function tokenRefresh(root, args, context, info) {
  const id = _.get(args, 'id');
  const token = await data.bots.tokenRefresh({ id });
  return { token };
}

module.exports = {
  all,
  one,
  update,
  create,
  tokenRefresh,
};
