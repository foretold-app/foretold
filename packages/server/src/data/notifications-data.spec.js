const data = require('./notifications-data');

describe('Notifications Data', () => {
  it('class should be a constructor', () => {
    expect(data.NotificationsData).toBeInstanceOf(Function);
  });
});
