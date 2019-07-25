const Sequelize = require('sequelize');

function getDB() {
  const env = process.env.NODE_ENV || 'development';
  const config = require(__dirname + '/../../config/config.json')[env];
  const db = {};

  const sequelize = config.use_env_variable
    ? new Sequelize(process.env[config.use_env_variable], config)
    : new Sequelize(config.database, config.username, config.password, config);

  db.sequelize = sequelize;

  return db;
}

module.exports = {
  getDB,
};

