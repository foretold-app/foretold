const resolver = jest.fn(() => (...args) => Promise.resolve(args));

module.exports = {
  resolver,
  DateType: {
    default: null
  }
};
