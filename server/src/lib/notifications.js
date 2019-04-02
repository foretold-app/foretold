const { IncomingWebhook } = require('@slack/client');

const url = process.env.SLACK_WEBHOOK_URL;
const webhook = new IncomingWebhook(url);
console.log(`SET UP WITH SLACK with webhook url: ${url}`);

/**
 * @param {object} message
 * @return {Promise.<*>}
 */
function notify(message) {
  if (process.env.NODE_ENV === 'development') {
    console.log('Notification sent to Slack: ', message);
    return Promise.resolve(true);
  }
  return webhook.send(message)
    .then((res) => {
      console.log('Notification sent to Slack: ', res);
      return res;
    })
    .catch((err) => {
      console.error('Error Sending Notification to Slack:', err);
      return Promise.reject(err);
    });
}

module.exports = {
  notify,
};
