const _ = require('lodash');

const { BotsData } = require('../../data');

const { Pagination } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Data } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Filter } = require('../../data/classes');

/**
 * @param {*} _root
 * @param {object} args
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.User>}
 */
async function create(_root, args, context, _info) {
  const input = _.get(args, 'input') || {};
  const userId = _.get(context, 'user.id', null);

  const data = new Data({
    ...input,
    userId,
  });

  return new BotsData().createOne(data);
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Models.BotID} args.id
 * @param {object} args.input
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<Models.User>}
 */
async function update(_root, args, _context, _info) {
  const id = _.get(args, 'id', null);
  const input = _.get(args, 'input') || {};

  const params = new Params({ id });
  const data = new Data(input);

  return new BotsData().updateOne(params, data);
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Models.AgentID} args.ownerId
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function all(_root, args, _context, _info) {
  const ownerId = _.get(args, 'ownerId', null);

  const filter = new Filter({ userId: ownerId });
  const pagination = new Pagination(args);
  const options = new Options();

  return new BotsData().getConnection(filter, pagination, options);
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<Model>}
 */
async function one(_root, args, _context, _info) {
  const id = _.get(args, 'id', null);
  const params = new Params({ id });
  return new BotsData().getOne(params);
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<{token: string}>}
 */
async function tokenRefresh(_root, args, _context, _info) {
  const id = _.get(args, 'id', null);
  const token = await new BotsData().tokenRefresh({ id });
  return { token };
}

module.exports = {
  all,
  one,
  update,
  create,
  tokenRefresh,
};
