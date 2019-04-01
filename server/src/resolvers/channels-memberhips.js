const data = require('../data');

/**
 * @param {*} root
 * @param {{channelId: string, agentId: string, role: string}} args
 * @returns {Promise<Models.ChannelMembership>}
 */
async function create(root, args) {
  return await data.channelsMembershipsData.createOne(
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
  return await data.channelsMembershipsData.updateOne(
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
  return await data.channelsMembershipsData.deleteOne(
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
async function all(root, args, context, info) {
  const channelId = root.id;
  const options = { channelId };
  return await data.channelsMembershipsData.getAll(options);
}

module.exports = {
  all,
  remove,
  create,
  update,
};
