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

/**
 * @param entity
 * @param entity.getCreationNotification
 * @param entity.getChannel
 * @param creator
 * @return {Promise<boolean>}
 */
async function creationNotification(entity, creator) {
  const notification = await entity.getCreationNotification(creator);
  const channel = await entity.getChannel();
  if (channel.isPublic) await notify(notification);
  return true;
}

/**
 * @param entity
 * @param entity.getUpdateNotifications
 * @param entity.getChannel
 * @param creator
 * @param data
 * @return {Promise<boolean>}
 */
async function updateNotification(entity, creator, data) {
  const notification = await entity.getUpdateNotifications(creator, data);
  const channel = await entity.getChannel();
  if (channel.isPublic) await notify(notification);
  return true;
}

module.exports = {
  notify,
  updateNotification,
  creationNotification,
};
