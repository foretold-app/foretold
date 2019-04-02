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

db[agent.name] = agent;
db[bot.name] = bot;
db[channelMembership.name] = channelMembership;
db[channel.name] = channel;
db[measurable.name] = measurable;
db[measurement.name] = measurement;
db[series.name] = series;
db[user.name] = user;

Object.keys(db).forEach(modelName => {
  if (db[modelName].associate) {
    console.log('Build association for model:', modelName);
    db[modelName].associate(db);
  }
});

module.exports = db;
