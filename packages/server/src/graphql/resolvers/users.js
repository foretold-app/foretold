const _ = require('lodash');

const { UsersData } = require('../../data');

const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Data } = require('../../data/classes');

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.UserID} args.id
 * @param {object} args.input
 * @param {Schema.Context} _context
 * @returns {Promise<Models.User>}
 */
async function update(root, args, _context) {
  const userId = _.get(args, 'id', null);
  const input = _.get(args, 'input') || {};

  const params = new Params({ id: userId });
  const data$ = new Data(input);

  return new UsersData().updateOne(params, data$);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.UserID} args.id
 * @param {object} args.input
 * @param {String} args.input.auth0AccessToken
 * @param {Schema.Context} _context
 * @returns {Promise<Models.User>}
 */
async function accessTokenUpdate(root, args, _context) {
  const userId = _.get(args, 'id', null);
  const input = _.get(args, 'input', null);

  const params = new Params({ id: userId });
  const data$ = new Data(input);

  return new UsersData().updateOne(params, data$);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.UserID} args.id
 * @param {Schema.Context} _context
 * @returns {Promise<Models.User>}
 */
async function one(root, args, _context) {
  const userId = _.get(args, 'id', null) || _.get(root, 'userId', null);
  const params = new Params({ id: userId });
  return new UsersData().getOne(params);
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
  return new UsersData().getOne(params, query, options);
}

/**
 * @param {object} _root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function count(_root, _args, _context, _info) {
  return new UsersData().getCount();
}

module.exports = {
  accessTokenUpdate,
  count,
  one,
  oneByAgentId,
  update,
};
