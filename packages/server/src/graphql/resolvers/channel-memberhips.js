const _ = require('lodash');

const { ChannelMembershipsData } = require('../../data');

const { Pagination } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Filter } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Data } = require('../../data/classes');

/**
 * @param {*} _root
 * @param {{input: {
 * channelId: Models.ChannelID,
 * agentId: Models.AgentID,
 * role: string,
 * }}} args
 * @param {Schema.Context} context
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function create(_root, args, context) {
  const input = _.get(args, 'input') || {};
  const inviterAgentId = _.get(context, 'agent.id', null);

  return new ChannelMembershipsData().upsertOne(new Params({
    channelId: input.channelId,
    agentId: input.agentId,
  }), {}, new Data({
    channelId: input.channelId,
    agentId: input.agentId,
    inviterAgentId,
    role: input.role,
  }));
}

/**
 * @param {*} _root
 * @param {{input: {
 * channelId: Models.ChannelID,
 * agentId: Models.AgentID,
 * role: string,
 * }}} args
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function update(_root, args) {
  const input = _.get(args, 'input') || {};

  return new ChannelMembershipsData().updateOne(new Params({
    channelId: input.channelId,
    agentId: input.agentId,
  }), new Data({
    channelId: input.channelId,
    agentId: input.agentId,
    role: input.role,
  }));
}

/**
 * @param _root
 * @param {{input: {
 * channelId: Models.ChannelID,
 * agentId: Models.AgentID,
 * }}} args
 * @returns {Promise<Models.ChannelMemberships | null>}
 */
async function remove(_root, args) {
  const input = _.get(args, 'input') || {};
  return new ChannelMembershipsData().deleteOne(new Params({
    channelId: input.channelId,
    agentId: input.agentId,
  }));
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
  const agentId = _.get(root, 'id', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const filter = new Filter({ agentId });
  const pagination = new Pagination();
  const options = new Options({ currentAgentId });

  return new ChannelMembershipsData().getAll(filter, pagination, options);
}

/**
 * @param {object | null} root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.ChannelMemberships[]>}
 */
async function allByChannelId(root, _args, context, _info) {
  const channelId = _.get(root, 'id', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const filter = new Filter({ channelId });
  const pagination = new Pagination();
  const options = new Options({ currentAgentId });

  return new ChannelMembershipsData().getAll(filter, pagination, options);
}

/**
 * @param {object | null} _root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function join(_root, args, context, _info) {
  const channelId = _.get(args, 'input.channelId', null);
  const agentId = _.get(context, 'agent.id', null);

  return new ChannelMembershipsData().join({
    channelId,
    agentId,
  });
}

/**
 * @param {object | null} _root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function leave(_root, args, context, _info) {
  const channelId = _.get(args, 'input.channelId', null);
  const agentId = _.get(context, 'agent.id', null);

  return new ChannelMembershipsData().leave({
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
  const channelId = _.get(root, 'id', null);
  const agentId = _.get(context, 'agent.id', null);

  return new ChannelMembershipsData().getOneOnlyRole(new Params({
    channelId,
    agentId,
  }));
}

/**
 * @param {object | null} root
 * @param {{ input: Schema.ChannelsInput }} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<string>}
 */
async function membershipCount(root, _args, _context, _info) {
  return _.get(root, 'membersCount', 0);
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
