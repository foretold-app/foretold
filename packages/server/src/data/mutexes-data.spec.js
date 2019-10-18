const data = require('./mutexes-data');

describe('Mutexes Data', () => {
  it('class should be a constructor', () => {
    expect(data.MutexesData).toBeInstanceOf(Function);
  });
});
