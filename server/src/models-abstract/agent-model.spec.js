const { AgentModel } = require('./agent-model');

describe('Agent Model', () => {
  it('class should be a constructor', () => {
    expect(AgentModel).toBeInstanceOf(Function);
  });
});
