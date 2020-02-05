const _ = require('lodash');

const { UsersData } = require('../../data');
const logger = require('../../lib/log');

const log = logger.module('middlewares/users');

/**
 * @todo: Do not use "context.user" to compare objects.
 * @param {object | null} root
 * @param {object} args
 * @param {object} args.id
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextUser(root, args, context, _info) {
  const id = _.get(args, 'id', null);

  log.trace('\x1b[36m ---> \x1b[0m Middleware (setContextUser)', { id });

  // @todo: Don't understand what this means!!!
  context.userAsObject = !!id
    ? await new UsersData().getOne({ id })
    : null;
}

module.exports = {
  setContextUser,
};
