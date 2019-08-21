const assert = require('assert');

/**
 * Look at this literal as on a structure.
 * @param {string} agentId
 * @return {Layers.withinJoinedChannels}
 */
module.exports.withinJoinedChannelsByChannelId = (agentId) => {
  assert(!!agentId, 'AgentId is required');
  return { as: 'channelId', agentId };
};

/**
 * @param {string} agentId
 * @return {Layers.withinJoinedChannels}
 */
module.exports.withinJoinedChannelsById = (agentId) => {
  assert(!!agentId, 'AgentId is required');
  return { as: 'id', agentId };
};

/**
 * @param {string} agentId
 * @return {Layers.withinJoinedChannels}
 */
module.exports.withinPublicAndJoinedChannelsByChannelId = (agentId) => {
  assert(!!agentId, 'AgentId is required');
  return { as: 'channelId', agentId };
};

/**
 * @param {string} agentId
 * @return {Layers.withinJoinedChannels}
 */
module.exports.withinPublicAndJoinedChannelsById = (agentId) => {
  assert(!!agentId, 'AgentId is required');
  return { as: 'id', agentId };
};
