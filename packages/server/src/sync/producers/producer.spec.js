const { Producer } = require('./producer');

describe('Producer', () => {
  it('returns an constructor', () => {
    expect(Producer).toBeInstanceOf(Function);
  });
});
