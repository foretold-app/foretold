const _ = require('lodash');

const data = require('../data');

const { Pagination } = require('../data/classes/pagination');
const { Filter } = require('../data/classes/filter');
const { Options } = require('../data/classes/options');

const { withinMeasurables } = require('../structures');

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.channelId
 * @param {string[]} args.measurableState
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function all(root, args, context, info) {
  const channelId = _.get(args, 'channelId');
  const measurableState = _.get(args, 'measurableState');
  const currentAgentId = _.get(context, 'agent.id');

  const withinMeasurables = withinMeasurables(measurableState, channelId);

  const filter = new Filter({ withinMeasurables });
  const pagination = new Pagination(args);
  const options = new Options({ currentAgentId });

  return data.agentMeasurables.getConnection(filter, pagination, options);
}

module.exports = {
  all,
};
