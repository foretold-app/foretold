const data = require('../data');

/**
 * @tested
 * @param root
 * @param values
 * @returns {Promise<Model>}
 */
async function create(root, values) {
  return await data.agentsChannelsData.createOne(values.channelId, values.agentId);
}

/**
 * @tested
 * @param root
 * @param values
 * @returns {Promise<Model>}
 */
async function remove(root, values) {
  return await data.agentsChannelsData.deleteOne(values.channelId, values.agentId);
}


module.exports = {
  remove,
  create,
};
