const Sequelize = require('sequelize');

const env = process.env.NODE_ENV || 'development';
const config = require(`${__dirname}/../../config/config.json`)[env];

function getDB() {
  const db = {};

  db.sequelize = config.use_env_variable && process.env[config.use_env_variable]
    ? new Sequelize(process.env[config.use_env_variable], config)
    : new Sequelize(config.database, config.username, config.password, config);

  return db;
}

module.exports = {
  getDB,
};
