const _ = require('lodash');

const { BotsData } = require('../../data');
const logger = require('../../lib/log');

const log = logger.module('middlewares/bots');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {object} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function setContextBot(root, args, context, info) {
  const id = _.get(args, 'id');
  log.trace('\x1b[36m ---> \x1b[0m Middleware (setContextBot)', { id });
  context.bot = id ? await new BotsData().getOne({ id }) : null;
}

module.exports = {
  setContextBot,
};
