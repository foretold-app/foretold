const data = require('../data');

/**
 * @param {*} root
 * @param {{channelId: string, agentId: string, role: string}} args
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function create(root, args) {
  return await data.channelMemberships.createOne(
    args.channelId,
    args.agentId,
    args.role
  );
}

/**
 * @param root
 * @param {{channelId: string, agentId: string, role: string}} args
 * @returns {Promise<Models.ChannelMemberships>}
 */
async function update(root, args) {
  return await data.channelMemberships.updateOne(
    args.channelId,
    args.agentId,
    args.role
  );
}

/**
 * @param root
 * @param {{channelId: string, agentId: string}} args
 * @returns {Promise<Models.ChannelMemberships | null>}
 */
async function remove(root, args) {
  return await data.channelMemberships.deleteOne(
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
