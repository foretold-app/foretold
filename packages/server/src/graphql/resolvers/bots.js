const _ = require('lodash');

const { BotsData } = require('../../data');

const { Pagination } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Data } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Query } = require('../../data/classes');
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
 * @param {Defs.BotID} args.id
 * @param {object} args.input
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<Models.User>}
 */
async function update(_root, args, _context, _info) {
  const botId = _.get(args, 'id', null);
  const input = _.get(args, 'input') || {};

  const params = new Params({ id: botId });
  const data = new Data(input);

  return new BotsData().updateOne(params, data);
}

/**
 * @param {*} root
 * @param {Defs.UserID} root.userId
 * @param {object} args
 * @param {Defs.AgentID} args.ownerId
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function all(root, args, _context, _info) {
  const ownerId = _.get(args, 'ownerId', null);

  const filter = new Filter({ userId: ownerId });
  const pagination = new Pagination(args);
  const options = new Options({ raw: true });

  return new BotsData().getConnection(filter, pagination, options);
}

/**
 * @param {*} root
 * @param {Defs.UserID} root.userId
 * @param {object} args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function allById(root, args, _context, _info) {
  const userId = _.get(root, 'id', null);

  const filter = new Filter({ userId });
  const pagination = new Pagination(args);
  const options = new Options({ raw: true });

  return new BotsData().getAll(filter, pagination, options);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<Model>}
 */
async function one(root, args, _context, _info) {
  const botId = _.get(args, 'id', null);
  const params = new Params({ id: botId });
  const query = new Query();
  const options = new Options({ raw: true });
  return new BotsData().getOne(params, query, options);
}

/**
 * @param {*} root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<Model>}
 */
async function oneByAgentId(root, _args, _context, _info) {
  const agentId = _.get(root, 'id', null);
  const params = new Params({ agentId });
  const query = new Query();
  const options = new Options({ raw: true });
  return new BotsData().getOne(params, query, options);
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<{token: string}>}
 */
async function tokenRefresh(_root, args, _context, _info) {
  const botId = _.get(args, 'id', null);
  const params = new Params({ id: botId });
  const token = await new BotsData().tokenRefresh(params);
  return { token };
}

/**
 * @param {object} _root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function count(_root, _args, _context, _info) {
  return new BotsData().getCount();
}

module.exports = {
  all,
  allById,
  count,
  create,
  one,
  oneByAgentId,
  tokenRefresh,
  update,
};
