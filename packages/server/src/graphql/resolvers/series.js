const _ = require('lodash');

const { SeriesData } = require('../../data');

const { Pagination } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Filter } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Data } = require('../../data/classes');

/**
 * @param {*} root
 * @param {object} args
 * @param {Defs.SeriesID} args.id
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function one(root, args, context, _info) {
  const seriesId = _.get(args, 'id', null) || _.get(root, 'seriesId', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const params = new Params({ id: seriesId });
  const query = new Query();
  const options = new Options({
    isAdmin: _.get(context, 'agent.isAdmin', null),
    agentId: currentAgentId,
    raw: true,
  });

  return new SeriesData().getOne(params, query, options);
}

/**
 * @todo: To use "getConnection" to limit queries.
 * @param {*} root
 * @param {object} args
 * @param {Defs.ChannelID} args.channelId
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function all(root, args, context, _info) {
  const channelId = _.get(args, 'channelId', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const filter = new Filter({ channelId });
  const pagination = new Pagination();
  const options = new Options({
    isAdmin: _.get(context, 'agent.isAdmin', null),
    agentId: currentAgentId,
    raw: true,
  });

  return new SeriesData().getAll(filter, pagination, options);
}

/**
 * @todo:
 * @param {*} root
 * @param {object} args
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function create(root, args, context, _info) {
  const agentId = _.get(context, 'agent.id', null);
  const datas = new Data({
    ...args.input,
    creatorId: agentId,
  });
  return new SeriesData().createOne(datas);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Defs.NotebookID} args.id
 * @param {object} args.input
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function update(root, args, _context, _info) {
  const seriesId = _.get(args, 'id', null);
  const input = _.get(args, 'input') || {};
  const params = new Params({ id: seriesId });
  return new SeriesData().updateOne(params, input);
}


module.exports = {
  create,
  one,
  all,
  update,
};
