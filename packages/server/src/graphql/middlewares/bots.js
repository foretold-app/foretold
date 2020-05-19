const _ = require('lodash');

const { BotsData } = require('../../data');
const logger = require('../../lib/log');

const log = logger.module('middlewares/bots');

const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Options } = require('../../data/classes');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {object} args.id
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextBot(root, args, context, _info) {
  const id = _.get(args, 'id', null);

  log.trace('\x1b[36m ---> \x1b[0m Middleware (setContextBot)', { id });

  const params = new Params({ id });
  const query = new Query();
  const options = new Options({ raw: true });
  context.bot = !!id
    ? await new BotsData().getOne(params, query, options)
    : null;
}

module.exports = {
  setContextBot,
};
