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
};

const Agent = {
  findById: findById,
};

module.exports = {
  Channel,
  Agent,
  AgentsChannels,
};
