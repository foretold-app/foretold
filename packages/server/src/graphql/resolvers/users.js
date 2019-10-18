const _ = require('lodash');

const data = require('../../data');

const { Params } = require('../../data/classes');
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
  const id = _.get(args, 'id');
  const input = _.get(args, 'input');
  const params = new Params({ id });
  const data$ = new Data(input);
  return data.users.updateOne(params, data$);
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
  const id = _.get(args, 'id');
  const input = _.get(args, 'input');
  const params = new Params({ id });
  const data$ = new Data(input);
  return data.users.updateOne(params, data$);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.UserID} args.id
 * @param {Schema.Context} context
 * @returns {Promise<Models.User>}
 */
async function one(root, args, context) {
  const { id } = args;
  return data.users.getOne({ id });
}

module.exports = {
  one,
  update,
  accessTokenUpdate,
};
