const _ = require('lodash');
const data = require('../data');

/**
 * @param {*} root
 * @param {{input: {channelId: Models.ObjectID, agentId: Models.ObjectID, role: string}}} args
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function create(root, args) {
  const input = args.input;
  return data.channelMemberships.createOne(
    input.channelId,
    input.agentId,
    input.role
  );
}

/**
 * @param {*} root
 * @param {{input: {channelId: Models.ObjectID, agentId: Models.ObjectID, role: string}}} args
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function update(root, args) {
  const input = args.input;
  return data.channelMemberships.updateOne(
    input.channelId,
    input.agentId,
    input.role
  );
}

/**
 * @param root
 * @param {{input: {channelId: Models.ObjectID, agentId: Models.ObjectID}}} args
 * @returns {Promise<Models.ChannelMemberships | null>}
 */
async function remove(root, args) {
  const input = args.input;
  return data.channelMemberships.deleteOne(
    input.channelId,
    input.agentId
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
  const agentId = root.id;
  const options = { agentId };
  return data.channelMemberships.getAll(options);
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
  const channelId = root.id;
  const options = { channelId };
  return data.channelMemberships.getAll(options);
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
