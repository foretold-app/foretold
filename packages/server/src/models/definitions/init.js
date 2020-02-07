const logger = require('../../lib/log');

const log = logger.module('models/init');

/* eslint-disable no-param-reassign */

/**
 * @todo: Fix this, do not use loading in this style.
 * Nobody loves magic when debugs.
 * Keep it readable.
 */
function init(db) {
  const agent = db.sequelize.import('./agent');
  const bot = db.sequelize.import('./bot');
  const channel = db.sequelize.import('./channel');
  const channelMembership = db.sequelize.import('./channel-membership');
  const measurable = db.sequelize.import('./measurable');
  const measurement = db.sequelize.import('./measurement');
  const user = db.sequelize.import('./user');

  // Clear classes
  const agentChannel = db.sequelize.import('./agent-channel');
  const agentMeasurable = db.sequelize.import('./agent-measurable');
  const bookmark = db.sequelize.import('./bookmark');
  const channelAgent = db.sequelize.import('./channel-agent');
  const feedItem = db.sequelize.import('./feed-item');
  const globalSetting = db.sequelize.import('./global-setting');
  const invitation = db.sequelize.import('./invitation');
  const mutex = db.sequelize.import('./mutex');
  const notebook = db.sequelize.import('./notebook');
  const notification = db.sequelize.import('./notification');
  const notificationStatus = db.sequelize.import('./notification-status');
  const preference = db.sequelize.import('./preference');
  const series = db.sequelize.import('./series');
  const template = db.sequelize.import('./template');
  const token = db.sequelize.import('./token');
  const vote = db.sequelize.import('./vote');

  db.Agent = agent;
  db.AgentChannel = agentChannel;
  db.AgentMeasurable = agentMeasurable;
  db.Bot = bot;
  db.Channel = channel;
  db.ChannelAgent = channelAgent;
  db.ChannelMemberships = channelMembership;
  db.FeedItem = feedItem;
  db.GlobalSetting = globalSetting;
  db.Invitation = invitation;
  db.Measurable = measurable;
  db.Measurement = measurement;
  db.Mutex = mutex;
  db.Notebook = notebook;
  db.Notification = notification;
  db.NotificationStatus = notificationStatus;
  db.Preference = preference;
  db.Series = series;
  db.Template = template;
  db.Token = token;
  db.User = user;
  db.Vote = vote;
  db.Bookmark = bookmark;

  const initList = [
    'Agent', 'Bot', 'ChannelMemberships', 'Channel', 'Measurable',
    'Series', 'User', 'Token', 'Preference',
    'NotificationStatus', 'FeedItem', 'Measurement',
    'AgentMeasurable', 'AgentChannel', 'Invitation', 'Mutex',
    'Notebook', 'ChannelAgent', 'Vote', 'Bookmark'
  ];

  // Associate All Models
  log.trace(' --- ');
  const built = [];
  const pass = [];
  initList.forEach((modelName) => {
    if (db[modelName].associate) {
      db[modelName].associate(db);
      built.push(modelName);
    } else {
      pass.push(modelName);
    }
  });
  log.trace(' > Build association:', built.join(', '));
  log.trace(' > Pass association:', pass.join(', '));
  log.trace(' --- ');
}

/* eslint-enable no-param-reassign */

module.exports = {
  init,
};
