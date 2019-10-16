const Sequelize = require('sequelize');

const logger = require('../lib/log');

const env = process.env.NODE_ENV || 'development';
const configSrc = require(`${__dirname}/../../config/config.json`)[env];

const log = logger.module('models/db');

const config = { ...configSrc };

config.logging = (...rest) => log.trace(rest[0]);

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
