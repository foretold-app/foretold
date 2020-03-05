const { NotificationStatusModel } = require('./notification-status-model');

describe('Notification Status Model', () => {
  it('class should be a constructor', () => {
    expect(NotificationStatusModel).toBeInstanceOf(Function);
  });
});
