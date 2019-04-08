const ModelPostgres = {
  channelIdsLiteral: jest.fn(() => true),
  measurableIdsLiteral: jest.fn(() => true),
};

const AgentModel = {
  ...ModelPostgres,
};

const ChannelMembershipModel = {
  ...ModelPostgres,
};

const ChannelModel = {
  ...ModelPostgres,
};

const MeasurableModel = {
  ...ModelPostgres,
};

const MeasurementModel = {
  ...ModelPostgres,
};

const SeriesModel = {
  ...ModelPostgres,
};

const UserModel = {
  ...ModelPostgres,
};

module.exports = {
  AgentModel,
  ChannelMembershipModel,
  ChannelModel,
  MeasurableModel,
  MeasurementModel,
  SeriesModel,
  UserModel,
};
