const data = require('./notification-statuses-data');

describe('Agent Notifications Data', () => {
  it('class should be a constructor', () => {
    expect(data.AgentNotificationsData).toBeInstanceOf(Function);
  });
});
