const _ = require('lodash');
const data = require('../../data');

const { Pagination } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Filter } = require('../../data/classes');

/**
 * @param {*} _root
 * @param {{input: {
 * channelId: Models.ObjectID,
 * agentId: Models.ObjectID,
 * role: string,
 * }}} args
 * @param {*} context
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function create(_root, args, context) {
  const input = _.get(args, 'input');
  const inviterAgentId = _.get(context, 'agent.id');
  return data.channelMemberships.upsertOne({
    channelId: input.channelId,
    agentId: input.agentId,
  }, {}, {
    channelId: input.channelId,
    agentId: input.agentId,
    inviterAgentId,
    role: input.role,
  });
}

/**
 * @param {*} _root
 * @param {{input: {
 * channelId: Models.ObjectID,
 * agentId: Models.ObjectID,
 * role: string,
 * }}} args
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function update(_root, args) {
  const input = _.get(args, 'input');
  return data.channelMemberships.updateOne({
    channelId: input.channelId,
    agentId: input.agentId,
  }, {
    channelId: input.channelId,
    agentId: input.agentId,
    role: input.role,
  });
}

/**
 * @param _root
 * @param {{input: {
 * channelId: Models.ObjectID,
 * agentId: Models.ObjectID,
 * }}} args
 * @returns {Promise<Models.ChannelMemberships | null>}
 */
async function remove(_root, args) {
  const input = _.get(args, 'input');
  return data.channelMemberships.deleteOne({
    channelId: input.channelId,
    agentId: input.agentId,
  });
}

/**
 * @param {object | null} root
 * @param {string} root.id
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.ChannelMemberships[]>}
 */
async function allByAgentId(root, _args, context, _info) {
  const agentId = _.get(root, 'id');
  const currentAgentId = _.get(context, 'agent.id');

  const filter = new Filter({ agentId });
  const pagination = new Pagination();
  const options = new Options({ currentAgentId });

  return data.channelMemberships.getAll(filter, pagination, options);
}

/**
 * @param {object | null} root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.ChannelMemberships[]>}
 */
async function allByChannelId(root, _args, context, _info) {
  const channelId = _.get(root, 'id');
  const currentAgentId = _.get(context, 'agent.id');

  const filter = new Filter({ channelId });
  const pagination = new Pagination();
  const options = new Options({ currentAgentId });

  return data.channelMemberships.getAll(filter, pagination, options);
}

/**
 * @param {object | null} _root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function join(_root, args, context, _info) {
  const channelId = _.get(args, 'input.channelId');
  const agentId = _.get(context, 'agent.id');
  return data.channelMemberships.join({
    channelId,
    agentId,
  });
}

/**
 * @param {object | null} _root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function leave(_root, args, context, _info) {
  const channelId = _.get(args, 'input.channelId');
  const agentId = _.get(context, 'agent.id');
  return data.channelMemberships.leave({
    channelId,
    agentId,
  });
}

/**
 * @param {object | null} root
 * @param {{ input: Schema.ChannelsInput }} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<string>}
 */
async function myRole(root, _args, context, _info) {
  const channelId = _.get(root, 'id');
  const agentId = _.get(context, 'agent.id');
  return data.channelMemberships.getOneOnlyRole({
    channelId,
    agentId,
  });
}

/**
 * @param {object | null} root
 * @param {{ input: Schema.ChannelsInput }} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<string>}
 */
async function membershipCount(root, _args, _context, _info) {
  const channelId = _.get(root, 'id');
  return data.channelMemberships.getCount({
    channelId,
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
  membershipCount,
};
