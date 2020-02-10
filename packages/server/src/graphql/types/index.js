const agentChannels = require('./agent-channels');
const agentMeasurables = require('./agent-measurables');
const agents = require('./agents');
const authentications = require('./authentications');
const bookmarks = require('./bookmarks');
const bots = require('./bots');
const channelMemberships = require('./channel-memberhips');
const channels = require('./channels');
const common = require('./common');
const feedItems = require('./feed-items');
const globalSettings = require('./global-settings');
const invitationStatus = require('./enums/invitation-status');
const invitations = require('./invitations');
const jwt = require('./scalars/jwt');
const measurables = require('./measurables');
const measurementCompetitorType = require(
  './enums/measurement-competitor-type',
);
const measurements = require('./measurements');
const mutexes = require('./mutexes');
const notebooks = require('./notebooks');
const permissions = require('./permissions');
const preferences = require('./preferences');
const scalars = require('./scalars');
const series = require('./series');
const users = require('./users');
const votes = require('./votes');

module.exports = {
  agentChannels,
  agentMeasurables,
  agents,
  authentications,
  bookmarks,
  bots,
  channelMemberships,
  channels,
  common,
  feedItems,
  globalSettings,
  invitationStatus,
  invitations,
  jwt,
  measurables,
  measurementCompetitorType,
  measurements,
  mutexes,
  notebooks,
  permissions,
  preferences,
  scalars,
  series,
  users,
  votes,
};
