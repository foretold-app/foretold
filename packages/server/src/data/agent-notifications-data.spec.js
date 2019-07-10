const data = require('./agent-notifications-data');

describe('Agent Notifications Data', () => {
  it('class should be a constructor', () => {
    expect(data.AgentNotificationsData).toBeInstanceOf(Function);
  });
});
