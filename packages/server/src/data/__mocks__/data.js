/* eslint-disable max-classes-per-file */

const dataBaseMethods = () => ({
  deleteOne: jest.fn(() => Promise.resolve(true)),
  updateOne: jest.fn(() => Promise.resolve(true)),
  getAll: jest.fn(() => Promise.resolve(true)),
  getConnection: jest.fn(() => Promise.resolve(true)),
  findAll: jest.fn(() => Promise.resolve(true)),
  updateAll: jest.fn(() => Promise.resolve(true)),
  getOne: jest.fn(() => Promise.resolve(true)),
  createOne: jest.fn(() => Promise.resolve(true)),
  upsertOne: jest.fn(() => Promise.resolve(true)),
});

const agentsDataMethods = {...dataBaseMethods()};
const AgentsData = jest.fn()
  .mockImplementation(() => agentsDataMethods);

const channelMembershipsDataMethods = {
  ...dataBaseMethods(),
  join: jest.fn(() => Promise.resolve(true)),
  leave: jest.fn(() => Promise.resolve(true)),
  getOneOnlyRole: jest.fn(() => Promise.resolve(true)),
};
const ChannelMembershipsData = jest.fn()
  .mockImplementation(() => channelMembershipsDataMethods);

const channelsDataMethods = {
  ...dataBaseMethods(),
  getConnection: jest.fn(() => Promise.resolve({
    getData: async () => {
      return true;
    },
  })),
  getCreatorByChannelId: jest.fn(() => Promise.resolve(true)),
  getAgentsByChannelId: jest.fn(() => Promise.resolve(true)),
};
const ChannelsData = jest.fn()
  .mockImplementation(() => channelsDataMethods);

const measurablesDataMethods = {
  ...dataBaseMethods(),
  unArchive: jest.fn(() => Promise.resolve(true)),
  archive: jest.fn(() => Promise.resolve(true)),
};
const MeasurablesData = jest.fn()
  .mockImplementation(() => measurablesDataMethods);

const measurementsDataMethods = { ...dataBaseMethods() };
const MeasurementsData = jest.fn()
  .mockImplementation(() => measurementsDataMethods);

const seriesDataMethods = { ...dataBaseMethods() };
const SeriesData = jest.fn()
  .mockImplementation(() => seriesDataMethods);

const usersDataMethods = { ...dataBaseMethods() };
const UsersData = jest.fn()
  .mockImplementation(() => usersDataMethods);

const botsDataMethods = { ...dataBaseMethods() };
const BotsData = jest.fn()
  .mockImplementation(() => botsDataMethods);

const tokensDataMethods = { ...dataBaseMethods() };
const TokensData = jest.fn()
  .mockImplementation(() => tokensDataMethods);

const preferencesDataMethods = { ...dataBaseMethods() };
const PreferencesData = jest.fn()
  .mockImplementation(() => preferencesDataMethods);

const templatesDataMethods = { ...dataBaseMethods() };
const TemplatesData = jest.fn()
  .mockImplementation(() => templatesDataMethods);

const notificationsDataMethods = { ...dataBaseMethods() };
const NotificationsData = jest.fn()
  .mockImplementation(() => notificationsDataMethods);

const notificationStatusesDataMethods = { ...dataBaseMethods() };
const NotificationStatusesData = jest.fn()
  .mockImplementation(() => notificationStatusesDataMethods);

const invitationsDataMethods = { ...dataBaseMethods() };
const InvitationsData = jest.fn()
  .mockImplementation(() => invitationsDataMethods);

const feedItemsDataMethods = { ...dataBaseMethods() };
const FeedItemsData = jest.fn()
  .mockImplementation(() => feedItemsDataMethods);

const globalSettingsDataMethods = { ...dataBaseMethods() };
const GlobalSettingsData = jest.fn()
  .mockImplementation(() => globalSettingsDataMethods);

const agentMeasurablesDataMethods = { ...dataBaseMethods() };
const AgentMeasurablesData = jest.fn()
  .mockImplementation(() => agentMeasurablesDataMethods);

const agentChannelsDataMethods = { ...dataBaseMethods() };
const AgentChannelsData = jest.fn()
  .mockImplementation(() => agentChannelsDataMethods);

const dataBaseMethod$ = { ...dataBaseMethods() };
const DataBase = jest.fn()
  .mockImplementation(() => dataBaseMethod$);

/* eslint-enable max-classes-per-file */

module.exports = {
  AgentsData,
  ChannelMembershipsData,
  ChannelsData,
  MeasurablesData,
  MeasurementsData,
  SeriesData,
  UsersData,
  BotsData,
  TokensData,
  PreferencesData,
  TemplatesData,
  NotificationsData,
  NotificationStatusesData,
  InvitationsData,
  FeedItemsData,
  GlobalSettingsData,
  AgentMeasurablesData,
  AgentChannelsData,
  DataBase,
  dataBaseMethods,
};
