const common = require('./common');

describe('Common Types', () => {
  it('common is an object', () => {
    expect(common).toBeInstanceOf(Object);
  });
  it('iAmOwner is an object', () => {
    expect(common.iAmOwner).toBeInstanceOf(Object);
  });
  it('isMe is an object', () => {
    expect(common.isMe).toBeInstanceOf(Object);
  });
});
