const _ = require('lodash');
const assert = require('assert');

/**
 * Look at these literals as on structures.
 */

/**
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

/**
 * @param {string[]} states
 * @param {Models.ObjectID} channelId
 * @return {Layers.withinMeasurables | null}
 */
module.exports.withinMeasurables = (states, channelId) => {
  if (!!states) assert(_.isArray(states), 'States should be an array.');
  if (!_.size(states) && !channelId) return null;
  return { as: 'measurableId', states, channelId };
};
