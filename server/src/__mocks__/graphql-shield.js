module.exports = {
  shield: jest.fn(() => Promise.resolve()),
  rule: jest.fn(() => () => Promise.resolve()),
  and: jest.fn(() => () => Promise.resolve()),
  allow: jest.fn(() => () => Promise.resolve()),
  or: jest.fn(() => () => Promise.resolve()),
};
