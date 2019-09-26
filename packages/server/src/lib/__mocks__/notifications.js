const notify = jest.fn(() => Promise.resolve(true));

module.exports = {
  notify,
};
