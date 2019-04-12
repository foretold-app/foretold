const data = require('./data-base');

describe('Data Base', () => {
  it('class should be a constructor', () => {
    expect(data.DataBase).toBeInstanceOf(Function);
  });
});
