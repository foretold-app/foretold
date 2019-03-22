const { IncomingWebhook } = require('@slack/client');
const url = process.env.SLACK_WEBHOOK_URL;
const webhook = new IncomingWebhook(url);
console.log(`SET UP WITH SLACK with webhook url: ${url}`)

// Send simple text to the webhook channel
function notify(message) {
  if (process.env.NODE_ENV === 'development') {
    console.log('Notification sent to Slack: ', message);
    return;
  }
  webhook.send(message, function (err, res) {
    if (err) {
      console.error('Error Sending Notification to Slack:', err);
    } else {
      console.log('Notification sent to Slack: ', res);
    }
  });
}

export { notify };
