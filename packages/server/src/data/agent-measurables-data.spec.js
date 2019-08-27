const data = require('./agent-measurables-data');

describe('Agent Measurables Data', () => {
  it('class should be a constructor', () => {
    expect(data.AgentMeasurablesData).toBeInstanceOf(Function);
  });
});
