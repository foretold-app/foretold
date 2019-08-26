const { MeasurablesData } = require('./measurables-data');
const { MeasurementsData } = require('./measurements-data');
const { UsersData } = require('./users-data');
const { SeriesData } = require('./series-data');
const { ChannelsData } = require('./channels-data');
const { ChannelMembershipsData } = require('./channel-memberships-data');
const { AgentsData } = require('./agents-data');
const { BotsData } = require('./bots-data');
const { AuthenticationData } = require('./authentication-data');
const { TokensData } = require('./tokens-data');
const { PreferencesData } = require('./preferences-data');
const { NotificationsData } = require('./notifications-data');
const { TemplatesData } = require('./templates-data');
const { AgentNotificationsData } = require('./agent-notifications-data');
const { InvitationsData } = require('./invitations-data');
const { FeedItemsData } = require('./feed-items-data');
const { GlobalSettingsData } = require('./global-settings-data');
const { AgentMeasurablesData } = require('./agent-measurables-data');

module.exports = {
  measurables: new MeasurablesData(),
  measurements: new MeasurementsData(),
  users: new UsersData(),
  series: new SeriesData(),
  channels: new ChannelsData(),
  channelMemberships: new ChannelMembershipsData(),
  agents: new AgentsData(),
  bots: new BotsData(),
  authentication: new AuthenticationData(),
  tokens: new TokensData(),
  preferences: new PreferencesData(),
  notifications: new NotificationsData(),
  templates: new TemplatesData(),
  agentNotifications: new AgentNotificationsData(),
  invitations: new InvitationsData(),
  feedItems: new FeedItemsData(),
  globalSettings: new GlobalSettingsData(),
  agentMeasurables: new AgentMeasurablesData(),
};
