const { AgentNotificationModel } = require('./agent-notification-model');

describe('Agent Notification Model', () => {
  it('class should be a constructor', () => {
    expect(AgentNotificationModel).toBeInstanceOf(Function);
  });
});
