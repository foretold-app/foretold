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
  findById: jest.fn(() => Promise.resolve(true)),
  findOne: jest.fn(() => Promise.resolve(true)),
  create: jest.fn(() => Promise.resolve(true)),
  findAll: jest.fn(() => Promise.resolve(true)),
};

const Agent = {
  findById: jest.fn(() => Promise.resolve(true)),
};

const User = {
  findOne: jest.fn(() => Promise.resolve(true)),
  create: jest.fn(() => Promise.resolve(true)),
  findById: jest.fn(() => Promise.resolve(true)),
  findOrCreate: jest.fn(() => Promise.resolve(true)),
};

const Measurable = {
  findOne: jest.fn(() => Promise.resolve(true)),
  create: jest.fn(() => Promise.resolve(true)),
  findById: jest.fn(() => Promise.resolve(true)),
  findOrCreate: jest.fn(() => Promise.resolve(true)),
  findAll: jest.fn(() => Promise.resolve(true)),
};

const Measurement = {
  findOne: jest.fn(() => Promise.resolve(true)),
  create: jest.fn(() => Promise.resolve(true)),
  findById: jest.fn(() => Promise.resolve(true)),
  findOrCreate: jest.fn(() => Promise.resolve(true)),
  findAll: jest.fn(() => Promise.resolve(true)),
};

const Series = {
  findOne: jest.fn(() => Promise.resolve(true)),
  create: jest.fn(() => Promise.resolve(true)),
  findById: jest.fn(() => Promise.resolve(true)),
  findOrCreate: jest.fn(() => Promise.resolve(true)),
  findAll: jest.fn(() => Promise.resolve(true)),
};

const sequelize = {
  Op: {
    or: 'orop',
    ne: 'neop',
    eq: 'eqop',
    and: 'andop',
    in: 'inop',
  }
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
