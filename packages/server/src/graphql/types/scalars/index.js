const cursor = require('./cursor');
const jwt = require('./jwt');
const { createObjectId } = require('./object-id');
const { createLimitedString } = require('./limited-string');

const notebookId = createObjectId('NotebookId');
const agentId = createObjectId('AgentId');
const channelId = createObjectId('ChannelId');
const userId = createObjectId('UserId');
const measurableId = createObjectId('MeasurableId');

const string0to255 = createLimitedString(0, 255);
const string3to255 = createLimitedString(3, 255);
const string0to16K = createLimitedString(0, 16*1024);
const string3to16K = createLimitedString(3, 16*1024);

module.exports = {
  cursor,
  jwt,

  notebookId,
  agentId,
  channelId,
  userId,
  measurableId,

  string0to255,
  string3to255,

  string0to16K,
  string3to16K,
};
