const logger = require('../lib/log');

const log = logger.module('models/init');

/* eslint-disable no-param-reassign */

/**
 * Nobody loves magic when debugs.
 * Keep it readable.
 */
function init(db) {
  const agent = db.sequelize.import('./agent');
  const bot = db.sequelize.import('./bot');
  const channelMembership = db.sequelize.import('./channel-membership');
  const channel = db.sequelize.import('./channel');
  const measurable = db.sequelize.import('./measurable');
  const measurement = db.sequelize.import('./measurement');
  const series = db.sequelize.import('./series');
  const user = db.sequelize.import('./user');
  const token = db.sequelize.import('./token');
  const preference = db.sequelize.import('./preference');
  const template = db.sequelize.import('./template');
  const notification = db.sequelize.import('./notification');
  const notificationStatus = db.sequelize.import('./notification-status');
  const feedItem = db.sequelize.import('./feed-item');
  const globalSetting = db.sequelize.import('./global-setting');
  const agentMeasurable = db.sequelize.import('./agent-measurable');
  const agentChannel = db.sequelize.import('./agent-channel');
  const invitation = db.sequelize.import('./invitation');
  const mutex = db.sequelize.import('./mutex');
  const notebook = db.sequelize.import('./notebook');

  db.Agent = agent;
  db.Bot = bot;
  db.ChannelMemberships = channelMembership;
  db.Channel = channel;
  db.Measurable = measurable;
  db.Measurement = measurement;
  db.Series = series;
  db.User = user;
  db.Token = token;
  db.Preference = preference;
  db.Template = template;
  db.Notification = notification;
  db.NotificationStatus = notificationStatus;
  db.FeedItem = feedItem;
  db.GlobalSetting = globalSetting;
  db.AgentMeasurable = agentMeasurable;
  db.AgentChannel = agentChannel;
  db.Invitation = invitation;
  db.Mutex = mutex;
  db.Notebook = notebook;

  const initList = [
    'Agent', 'Bot', 'ChannelMemberships', 'Channel', 'Measurable',
    'Series', 'User', 'Token', 'Preference', 'Template', 'Notification',
    'NotificationStatus', 'FeedItem', 'GlobalSetting', 'Measurement',
    'AgentMeasurable', 'AgentChannel', 'Invitation', 'Mutex',
    'Notebook',
  ];

  // Associate All Models
  log.trace(' --- ');
  initList.forEach((modelName) => {
    if (db[modelName].associate) {
      log.trace(' > Build association for model:', modelName);
      db[modelName].associate(db);
    } else {
      log.trace(' > Pass association for model:', modelName);
    }
  });
  log.trace(' --- ');
}

/* eslint-enable no-param-reassign */

module.exports = {
  init,
};
