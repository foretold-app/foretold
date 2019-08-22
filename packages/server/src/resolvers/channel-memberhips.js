const _ = require('lodash');
const data = require('../data');

const { Pagination } = require('../data/classes/pagination');
const { Options } = require('../data/classes/options');
const { Filter } = require('../data/classes/filter');

/**
 * @param {*} root
 * @param {{input: {channelId: Models.ObjectID, agentId: Models.ObjectID, role: string}}} args
 * @param {*} context
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function create(root, args, context) {
  const input = _.get(args, 'input');
  const inviterAgentId = _.get(context, 'agent.id');
  return data.channelMemberships.createOne2(
    input.channelId,
    input.agentId,
    inviterAgentId,
    input.role,
  );
}

/**
 * @param {*} root
 * @param {{input: {channelId: Models.ObjectID, agentId: Models.ObjectID, role: string}}} args
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function update(root, args) {
  const input = _.get(args, 'input');
  return data.channelMemberships.updateOne2(
    input.channelId,
    input.agentId,
    input.role,
  );
}

/**
 * @param root
 * @param {{input: {channelId: Models.ObjectID, agentId: Models.ObjectID}}} args
 * @returns {Promise<Models.ChannelMemberships | null>}
 */
async function remove(root, args) {
  const input = _.get(args, 'input');
  return data.channelMemberships.deleteOne2(
    input.channelId,
    input.agentId,
  );
}

/**
 * @param {object | null} root
 * @param {string} root.id
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.ChannelMemberships[]>}
 */
async function allByAgentId(root, args, context, info) {
  const agentId = _.get(root, 'id');
  const currentAgentId = _.get(context, 'agent.id');

  const filter = new Filter({ agentId });
  const pagination = new Pagination();
  const options = new Options({ currentAgentId });

  return data.channelMemberships.getAll(filter, pagination, options);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.ChannelMemberships[]>}
 */
async function allByChannelId(root, args, context, info) {
  const channelId = _.get(root, 'id');
  const currentAgentId = _.get(context, 'agent.id');

  const filter = new Filter({ channelId });
  const pagination = new Pagination();
  const options = new Options({ currentAgentId });

  return data.channelMemberships.getAll(filter, pagination, options);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function join(root, args, context, info) {
  const channelId = _.get(args, 'input.channelId');
  const agentId = _.get(context, 'agent.id');
  return data.channelMemberships.join({
    channelId,
    agentId,
  });
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function leave(root, args, context, info) {
  const channelId = _.get(args, 'input.channelId');
  const agentId = _.get(context, 'agent.id');
  return data.channelMemberships.leave({
    channelId,
    agentId,
  });
}

/**
 * @param {object | null} root
 * @param {{ input: Schema.ChannelsInput }} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<string>}
 */
async function myRole(root, args, context, info) {
  const channelId = _.get(root, 'id');
  const agentId = _.get(context, 'agent.id');
  return data.channelMemberships.getOneOnlyRole({
    channelId,
    agentId,
  });
}

module.exports = {
  allByAgentId,
  allByChannelId,
  remove,
  create,
  update,
  join,
  leave,
  myRole,
};
