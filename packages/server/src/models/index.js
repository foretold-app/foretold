const db = require('./db');

/**
 * Nobody loves magic when debugs.
 * Keep it readable.
 */

const agent = db.sequelize.import('./agent');
const bot = db.sequelize.import('./bot');
const channelMembership = db.sequelize.import('./channel-membership');
const channel = db.sequelize.import('./channel');
const measurable = db.sequelize.import('./measurable');
const measurement = db.sequelize.import('./measurement');
const series = db.sequelize.import('./series');
const user = db.sequelize.import('./user');
const token = db.sequelize.import('./token');

db.Agent = agent;
db.Bot = bot;
db.ChannelMemberships = channelMembership;
db.Channel = channel;
db.Measurable = measurable;
db.Measurement = measurement;
db.Series = series;
db.User = user;
db.Token = token;

Object.keys(db).forEach(modelName => {
  if (db[modelName].associate) {
    console.log('Build association for model:', modelName);
    db[modelName].associate(db);
  }
});

module.exports = db;
