const Sequelize = require('sequelize');
const env = process.env.NODE_ENV || 'development';
const config = require(__dirname + '/../../config/config.json')[env];
const db = {};

const extendedConfig = {
  ...config,
  operatorsAliases: false,
};

if (config.use_env_variable) {
  var sequelize = new Sequelize(process.env[config.use_env_variable], extendedConfig);
} else {
  var sequelize = new Sequelize(config.database, config.username, config.password, extendedConfig);
}

db.sequelize = sequelize;
db.Sequelize = Sequelize;

module.exports = db;
