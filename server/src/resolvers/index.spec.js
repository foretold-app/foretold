jest.mock('../models');
jest.mock('../lib/notifications');

const index = require('./index');

describe('channels', () => {
  it('class should be object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
