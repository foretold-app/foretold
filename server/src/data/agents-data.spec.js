const data = require('./agents-data');

describe('Agents Data', () => {
  it('class should be a constructor', () => {
    expect(data.AgentsData).toBeInstanceOf(Function);
  });
});
