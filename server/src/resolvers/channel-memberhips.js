const data = require('../data');

/**
 * @param {*} root
 * @param {{input: {channelId: string, agentId: string, role: string}}} args
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function create(root, args) {
  const input = args.input;
  return await data.channelMemberships.createOne(
    input.channelId,
    input.agentId,
    input.role
  );
}

/**
 * @param {*} root
 * @param {{input: {channelId: string, agentId: string, role: string}}} args
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function update(root, args) {
  const input = args.input;
  return await data.channelMemberships.updateOne(
    input.channelId,
    input.agentId,
    input.role
  );
}

/**
 * @param root
 * @param {{input: {channelId: string, agentId: string}}} args
 * @returns {Promise<Models.ChannelMemberships | null>}
 */
async function remove(root, args) {
  const input = args.input;
  return await data.channelMemberships.deleteOne(
    input.channelId,
    input.agentId
  );
}

/**
 * @param {object | null} root
 * @param {string} root.id
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.ChannelMemberships[]>}
 */
async function allByAgentId(root, args, context, info) {
  const agentId = root.id;
  const options = { agentId };
  return await data.channelMemberships.getAll(options);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.ChannelMemberships[]>}
 */
async function allByChannelId(root, args, context, info) {
  const channelId = root.id;
  const options = { channelId };
  return await data.channelMemberships.getAll(options);
}

module.exports = {
  allByAgentId,
  allByChannelId,
  remove,
  create,
  update,
};
