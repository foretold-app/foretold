const { ModelPostgres } = require('./model-postgres');

describe('Model Postgres', () => {
  it('class should be a constructor', () => {
    expect(ModelPostgres).toBeInstanceOf(Function);
  });
});
