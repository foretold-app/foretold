const { parentPort, workerData } = require('worker_threads');

const { AgentChannelsData } = require('../agent-channels-data');
const agentChannels = new AgentChannelsData();

const { agentId, channelId } = workerData;

agentChannels.primaryPointScore2(agentId, channelId).then((result) => {
  parentPort.postMessage(result);
});
