const data = require('./bots-data');

describe('Bots Data', () => {
  it('class should be a constructor', () => {
    expect(data.BotsData).toBeInstanceOf(Function);
  });
});
