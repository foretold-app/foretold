const { MutexModel } = require('./mutex-model');

describe('Mutex Model', () => {
  it('class should be a constructor', () => {
    expect(MutexModel).toBeInstanceOf(Function);
  });
});
