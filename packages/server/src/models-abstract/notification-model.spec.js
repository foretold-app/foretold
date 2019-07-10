const { NotificationModel } = require('./notification-model');

describe('Notification Model', () => {
  it('class should be a constructor', () => {
    expect(NotificationModel).toBeInstanceOf(Function);
  });
});
