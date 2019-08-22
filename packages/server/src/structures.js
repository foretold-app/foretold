const assert = require('assert');

/**
 * Look at this literal as on a structure.
 * @param {Models.ObjectID} agentId
 * @return {Layers.withinJoinedChannels}
 */
module.exports.withinJoinedChannelsByChannelId = (agentId) => {
  assert(!!agentId, 'AgentId is required');
  return { as: 'channelId', agentId };
};

/**
 * @param {Models.ObjectID} agentId
 * @return {Layers.withinJoinedChannels}
 */
module.exports.withinJoinedChannelsById = (agentId) => {
  assert(!!agentId, 'AgentId is required');
  return { as: 'id', agentId };
};

/**
 * @param {Models.ObjectID} agentId
 * @return {Layers.withinPublicAndJoinedChannels}
 */
module.exports.withinPublicAndJoinedChannelsByChannelId = (agentId) => {
  assert(!!agentId, 'AgentId is required');
  return { as: 'channelId', agentId };
};

/**
 * @param {Models.ObjectID} agentId
 * @return {Layers.withinPublicAndJoinedChannels}
 */
module.exports.withinPublicAndJoinedChannelsById = (agentId) => {
  assert(!!agentId, 'AgentId is required');
  return { as: 'id', agentId };
};

/**
 * @return {Layers.withinPublicChannels}
 */
module.exports.withinPublicChannelsByChannelId = () => {
  return { as: 'channelId' };
};

/**
 * @return {Layers.withinPublicChannels}
 */
module.exports.withinPublicChannelsById = () => {
  return { as: 'id' };
};
