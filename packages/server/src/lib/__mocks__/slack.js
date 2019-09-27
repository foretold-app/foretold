const sendNotificationToSlack = jest.fn(() => Promise.resolve(true));

module.exports = {
  sendNotificationToSlack,
};
