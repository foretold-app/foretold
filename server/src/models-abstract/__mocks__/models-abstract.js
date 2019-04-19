const ModelPostgres = {
  channelIdsLiteral: jest.fn(() => 'channelIdsLiteral'),
  measurableIdsLiteral: jest.fn(() => 'measurableIdsLiteral'),
  deleteOne: jest.fn(() => Promise.resolve(true)),
  updateOne: jest.fn(() => Promise.resolve(true)),
  getAll: jest.fn(() => Promise.resolve(true)),
  findAll: jest.fn(() => Promise.resolve(true)),
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

module.exports = {
  AgentModel,
  ChannelMembershipModel,
  ChannelModel,
  MeasurableModel,
  MeasurementModel,
  SeriesModel,
  UserModel,
  BotModel,
};
