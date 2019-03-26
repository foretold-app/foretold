const findOne = jest.fn(() => Promise.resolve(true));
const create = jest.fn(() => Promise.resolve(true));
const findById = jest.fn(() => Promise.resolve(true));
const destroy = jest.fn(() => Promise.resolve(true));

const AgentsChannels = {
  findOne: findOne,
  create: create,
  destroy: destroy,
};

const Channel = {
  findById: findById,
  findOne: jest.fn(() => Promise.resolve(true)),
  create: jest.fn(() => Promise.resolve(true)),
};

const Agent = {
  findById: findById,
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
};

const Measurement = {
  findOne: jest.fn(() => Promise.resolve(true)),
  create: jest.fn(() => Promise.resolve(true)),
  findById: jest.fn(() => Promise.resolve(true)),
  findOrCreate: jest.fn(() => Promise.resolve(true)),
};

const Series = {
  findOne: jest.fn(() => Promise.resolve(true)),
  create: jest.fn(() => Promise.resolve(true)),
  findById: jest.fn(() => Promise.resolve(true)),
  findOrCreate: jest.fn(() => Promise.resolve(true)),
};

module.exports = {
  User,
  Agent,
  Channel,
  AgentsChannels,
  Measurable,
  Measurement,
  Series,
};
