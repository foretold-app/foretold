const ChannelMemberships = {
  findOne: jest.fn(() => Promise.resolve(true)),
  create: jest.fn(() => Promise.resolve(true)),
  destroy: jest.fn(() => Promise.resolve(true)),
  findAll: jest.fn(() => Promise.resolve(true)),
  ROLE: {
    ADMIN: 'ADMIN',
    VIEWER: 'VIEWER',
  }
};

const Channel = {
  findByPk: jest.fn(() => Promise.resolve(true)),
  findOne: jest.fn(() => Promise.resolve(true)),
  create: jest.fn(() => Promise.resolve(true)),
  findAll: jest.fn(() => Promise.resolve(true)),
};

const Agent = {
  findByPk: jest.fn(() => Promise.resolve(true)),
};

const User = {
  findOne: jest.fn(() => Promise.resolve(true)),
  create: jest.fn(() => Promise.resolve(true)),
  findByPk: jest.fn(() => Promise.resolve(true)),
  findOrCreate: jest.fn(() => Promise.resolve(true)),
};

const Measurable = {
  findOne: jest.fn(() => Promise.resolve(true)),
  create: jest.fn(() => Promise.resolve(true)),
  findByPk: jest.fn(() => Promise.resolve(true)),
  findOrCreate: jest.fn(() => Promise.resolve(true)),
  findAll: jest.fn(() => Promise.resolve(true)),
};

const Measurement = {
  findOne: jest.fn(() => Promise.resolve(true)),
  create: jest.fn(() => Promise.resolve(true)),
  findByPk: jest.fn(() => Promise.resolve(true)),
  findOrCreate: jest.fn(() => Promise.resolve(true)),
  findAll: jest.fn(() => Promise.resolve(true)),
};

const Series = {
  findOne: jest.fn(() => Promise.resolve(true)),
  create: jest.fn(() => Promise.resolve(true)),
  findByPk: jest.fn(() => Promise.resolve(true)),
  findOrCreate: jest.fn(() => Promise.resolve(true)),
  findAll: jest.fn(() => Promise.resolve(true)),
};

const sequelize = {
};

module.exports = {
  User,
  Agent,
  Channel,
  ChannelMemberships,
  Measurable,
  Measurement,
  Series,
  sequelize,
};
