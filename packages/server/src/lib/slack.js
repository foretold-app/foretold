const util = require('util');
const { IncomingWebhook } = require('@slack/client');

/**
 * @param {object} message
 * @return {Promise.<*>}
 */
function sendNotificationToSlack(message) {
  const url = process.env.SLACK_WEBHOOK_URL;
  const webhook = new IncomingWebhook(url);
  console.log(`SET UP WITH SLACK with webhook url: ${url}`);

  console.log(
    'Notification sent to Slack:',
    util.inspect(message, {
      showHidden: false,
      depth: null,
      colors: true
    }),
  );

  if (process.env.NODE_ENV === 'development') {
    return Promise.resolve(true);
  }

  return webhook.send(message)
    .then((res) => {
      console.log('Notification sent to Slack result: ', res);
      return res;
    })
    .catch((err) => {
      console.error('Error Sending Notification to Slack:', err);
      return Promise.reject(err);
    });
}

module.exports = {
  sendNotificationToSlack,
};
