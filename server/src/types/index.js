const agents = require('./agents');
const channels = require('./channels');
const channelMemberships = require('./channel-memberhips');
const measurables = require('./measurables');
const measurements = require('./measurements');
const series = require('./series');
const users = require('./users');
const bots = require('./bots');
const permissions = require('./permissions');
const authentications = require('./authentications');

module.exports = {
  bots,
  agents,
  channels,
  channelMemberships,
  measurables,
  measurements,
  series,
  users,
  permissions,
  authentications,
};
