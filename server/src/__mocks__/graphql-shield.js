module.exports = {
  shield: jest.fn(() => Promise.resolve()),
  rule: jest.fn(() => () => Promise.resolve()),
};
