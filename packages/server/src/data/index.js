const { MeasurablesData } = require('./measurables-data');
const { MeasurementsData } = require('./measurements-data');
const { UsersData } = require('./users-data');
const { SeriesData } = require('./series-data');
const { ChannelsData } = require('./channels-data');
const { ChannelMembershipsData } = require('./channel-memberships-data');
const { AgentsData } = require('./agents-data');
const { BotsData } = require('./bots-data');
const { TokensData } = require('./tokens-data');
const { PreferencesData } = require('./preferences-data');
const { NotificationsData } = require('./notifications-data');
const { TemplatesData } = require('./templates-data');
const { NotificationStatusesData } = require('./notification-statuses-data');
const { InvitationsData } = require('./invitations-data');
const { FeedItemsData } = require('./feed-items-data');
const { GlobalSettingsData } = require('./global-settings-data');
const { AgentMeasurablesData } = require('./agent-measurables-data');
const { AgentChannelsData } = require('./agent-channels-data');
const { MutexesData } = require('./mutexes-data');
const { NotebooksData } = require('./notebooks-data');

module.exports = {
  MeasurablesData,
  MeasurementsData,
  UsersData,
  SeriesData,
  ChannelsData,
  ChannelMembershipsData,
  AgentsData,
  BotsData,
  TokensData,
  PreferencesData,
  NotificationsData,
  TemplatesData,
  NotificationStatusesData,
  InvitationsData,
  FeedItemsData,
  GlobalSettingsData,
  AgentMeasurablesData,
  AgentChannelsData,
  MutexesData,
  NotebooksData,

  // @todo: Repair it, do not provide instances.
  measurables: new MeasurablesData(),
  measurements: new MeasurementsData(),
  users: new UsersData(),
  series: new SeriesData(),
  channels: new ChannelsData(),
  channelMemberships: new ChannelMembershipsData(),
  agents: new AgentsData(),
  bots: new BotsData(),
  tokens: new TokensData(),
  preferences: new PreferencesData(),
  notifications: new NotificationsData(),
  templates: new TemplatesData(),
  notificationStatuses: new NotificationStatusesData(),
  invitations: new InvitationsData(),
  feedItems: new FeedItemsData(),
  globalSettings: new GlobalSettingsData(),
  agentMeasurables: new AgentMeasurablesData(),
  agentChannels: new AgentChannelsData(),
};
