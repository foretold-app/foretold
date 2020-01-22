const logger = require('../lib/log');

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
  const channelMembership = db.sequelize.import('./channel-membership');
  const channel = db.sequelize.import('./channel');
  const measurable = db.sequelize.import('./measurable');
  const measurement = db.sequelize.import('./measurement');
  const series = db.sequelize.import('./series');
  const user = db.sequelize.import('./user');
  const token = db.sequelize.import('./token');
  const preference = db.sequelize.import('./preference');
  const notificationStatus = db.sequelize.import('./notification-status');
  const feedItem = db.sequelize.import('./feed-item');
  const agentMeasurable = db.sequelize.import('./agent-measurable');
  const agentChannel = db.sequelize.import('./agent-channel');
  const invitation = db.sequelize.import('./invitation');
  const mutex = db.sequelize.import('./mutex');
  const notebook = db.sequelize.import('./notebook');

  // Clear classes
  const notification = db.sequelize.import('./notification');
  const template = db.sequelize.import('./template');
  const globalSetting = db.sequelize.import('./global-setting');
  const channelAgent = db.sequelize.import('./channel-agent');
  const votes = db.sequelize.import('./votes');

  db.Agent = agent;
  db.AgentMeasurable = agentMeasurable;
  db.AgentChannel = agentChannel;
  db.Bot = bot;
  db.ChannelMemberships = channelMembership;
  db.Channel = channel;
  db.FeedItem = feedItem;
  db.GlobalSetting = globalSetting;
  db.Invitation = invitation;
  db.Measurable = measurable;
  db.Measurement = measurement;
  db.Mutex = mutex;
  db.Notification = notification;
  db.NotificationStatus = notificationStatus;
  db.Notebook = notebook;
  db.User = user;
  db.Preference = preference;
  db.Series = series;
  db.Token = token;
  db.Template = template;
  db.ChannelAgent = channelAgent;
  db.Votes = votes;

  const initList = [
    'Agent', 'Bot', 'ChannelMemberships', 'Channel', 'Measurable',
    'Series', 'User', 'Token', 'Preference',
    'NotificationStatus', 'FeedItem', 'Measurement',
    'AgentMeasurable', 'AgentChannel', 'Invitation', 'Mutex',
    'Notebook', 'ChannelAgent', 'Votes',
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
