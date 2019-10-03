const _ = require('lodash');

const data = require('../../data');

const { Params } = require('../../data/classes/params');
const { Data } = require('../../data/classes/data');

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
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
 * @param {Schema.Context} context
 * @returns {Promise<Models.User>}
 */
async function one(root, args, context) {
  const { id, auth0Id } = args;
  if (context.user) {
    return context.user;
  } if (id) {
    return data.users.getOne({ id });
  } if (auth0Id) {
    return data.users.getUserByAuth0Id(auth0Id);
  }
}

module.exports = {
  one,
  update,
};
