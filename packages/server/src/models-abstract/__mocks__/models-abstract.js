const Op = {
  or: 'orop',
  ne: 'neop',
  eq: 'eqop',
  and: 'andop',
  in: 'inop',
};

const ModelPostgres = {
  Op,
  _publicAndJoinedChannelsLiteral: jest.fn(() => 'channelIdsLiteral'),
  _measurablesInPublicAndJoinedChannelsLiteral: jest.fn(() => 'measurableIdsLiteral'),
  deleteOne: jest.fn(() => Promise.resolve(true)),
  updateOne: jest.fn(() => Promise.resolve(true)),
  getAll: jest.fn(() => Promise.resolve(true)),
  getAllWithConnections: jest.fn(() => Promise.resolve(true)),
  findAll: jest.fn(() => Promise.resolve(true)),
  updateAll: jest.fn(() => Promise.resolve(true)),
  getOne: jest.fn(() => Promise.resolve(true)),
  createOne: jest.fn(() => Promise.resolve(true)),
  upsertOne: jest.fn(() => Promise.resolve(true)),
};

const AgentModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const ChannelMembershipModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const ChannelModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const MeasurableModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const MeasurementModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const SeriesModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const UserModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const BotModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const TokenModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const PreferenceModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const TemplateModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const NotificationModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const AgentNotificationModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const InvitationModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const FeedItemModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const GlobalSettingModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const AgentMeasurableModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const AgentChannelModel = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

const Model = jest.fn().mockImplementation(() => ({
  ...ModelPostgres,
}));

module.exports = {
  AgentModel,
  ChannelMembershipModel,
  ChannelModel,
  MeasurableModel,
  MeasurementModel,
  SeriesModel,
  UserModel,
  BotModel,
  TokenModel,
  PreferenceModel,
  TemplateModel,
  NotificationModel,
  AgentNotificationModel,
  InvitationModel,
  FeedItemModel,
  GlobalSettingModel,
  AgentMeasurableModel,
  AgentChannelModel,
  Model,
};
