const resolver = jest.fn(() => () => Promise.resolve(true));

module.exports = {
  resolver,
};
