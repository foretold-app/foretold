const { IncomingWebhook } = require('@slack/client');
const url = process.env.SLACK_WEBHOOK_URL;
const webhook = new IncomingWebhook(url);

// Send simple text to the webhook channel
function notify(message) {
webhook.send(message, function(err, res) {
    if (err) {
        console.error('Error Sending Notification to Slack:', err);
    } else {
        console.log('Notification sent to Slack: ', res);
    }
});

}
export {notify};