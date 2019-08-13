const _ = require('lodash');

const data = require('../data');

const { Pagination } = require('../data/classes/pagination');
const { Params } = require('../data/classes/params');
const { Data } = require('../data/classes/data');
const { Options } = require('../data/classes/options');
const { Filter } = require('../data/classes/filter');

/**
 * @param {*} root
 * @param {object} args
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.User>}
 */
async function create(root, args, context, info) {
  const datas = new Data({
    ...args.input,
    userId: _.get(context, 'user.id'),
  });
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
  const params = new Params({ id: args.id });
  const data = new Data(args.input);
  return data.bots.updateOne(params, data);
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
  const filter = new Filter({ userId: _.get(args, 'ownerId') });
  const pagination = new Pagination(args);
  const options = new Options();

  const connection = await data.bots.getConnection(filter, pagination, options);
  return connection.getData();
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
  const params = new Params({ id });
  return data.bots.getOne(params);
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
