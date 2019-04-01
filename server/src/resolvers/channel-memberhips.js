const data = require('../data');

/**
 * @param {*} root
 * @param {{channelId: string, agentId: string, role: string}} args
 * @returns {Promise<Models.ChannelMembership>}
 */
async function create(root, args) {
  return await data.channelMembershipsData.createOne(
    args.channelId,
    args.agentId,
    args.role
  );
}

/**
 * @param root
 * @param {{channelId: string, agentId: string, role: string}} args
 * @returns {Promise<Models.ChannelMembership>}
 */
async function update(root, args) {
  return await data.channelMembershipsData.updateOne(
    args.channelId,
    args.agentId,
    args.role
  );
}

/**
 * @param root
 * @param {{channelId: string, agentId: string}} args
 * @returns {Promise<Models.ChannelMembership | null>}
 */
async function remove(root, args) {
  return await data.channelMembershipsData.deleteOne(
    args.channelId,
    args.agentId
  );
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.ChannelMembership[]>}
 */
async function allByAgentId(root, args, context, info) {
  const agentId = root.id;
  const options = { agentId };
  return await data.channelMembershipsData.getAll(options);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.ChannelMembership[]>}
 */
async function allByChannelId(root, args, context, info) {
  const channelId = root.id;
  const options = { channelId };
  return await data.channelMembershipsData.getAll(options);
}

module.exports = {
  allByAgentId,
  allByChannelId,
  remove,
  create,
  update,
};
