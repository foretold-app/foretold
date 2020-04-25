const _ = require('lodash');

const { SeriesData } = require('../../data');
const logger = require('../../lib/log');

const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Options } = require('../../data/classes');

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
async function setContextSeries(root, args, context, _info) {
  const seriesId = _.get(args, 'id', null);

  log.trace('\x1b[36m ---> \x1b[0m Middleware (setContexSeries)', { seriesId });

  const params = new Params({ id: seriesId });
  const query = new Query();
  const options = new Options({ raw: true });

  context.series = !!seriesId
    ? await new SeriesData().getOne(params, query, options)
    : null;
}

module.exports = {
  setContextSeries,
};
