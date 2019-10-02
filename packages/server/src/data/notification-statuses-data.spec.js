const data = require('./notification-statuses-data');

describe('Notification Statuses Data', () => {
  it('class should be a constructor', () => {
    expect(data.NotificationStatusesData).toBeInstanceOf(Function);
  });
});
