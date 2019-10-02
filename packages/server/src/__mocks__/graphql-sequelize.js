const resolver = jest.fn(() => (...args) => Promise.resolve(args));
const createConnection = jest.fn(() => (...args) => Promise.resolve(args));

module.exports = {
  resolver,
  DateType: {
    default: null,
  },
  createConnection,
};
