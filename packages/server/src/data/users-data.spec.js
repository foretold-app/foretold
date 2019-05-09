const data = require('./users-data');

describe('Data Base', () => {
  it('class should be a constructor', () => {
    expect(data.UsersData).toBeInstanceOf(Function);
  });
});
