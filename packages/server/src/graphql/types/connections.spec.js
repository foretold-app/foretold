const connections = require('./connections');

describe('Connections Types', () => {
  it('returns an object', () => {
    expect(connections).toBeInstanceOf(Object);
  });
});
