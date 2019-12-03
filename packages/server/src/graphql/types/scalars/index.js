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

const string0to512 = createLimitedString(0, 512);
const string3to512 = createLimitedString(3, 512);

const string0to4K = createLimitedString(0, 4 * 1024);
const string3to4K = createLimitedString(3, 4 * 1024);

const string0to128K = createLimitedString(0, 128 * 1024);
const string3to128K = createLimitedString(3, 128 * 1024);

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

  string0to512,
  string3to512,

  string0to4K,
  string3to4K,

  string0to128K,
  string3to128K,
};
