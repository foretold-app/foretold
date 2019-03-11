const _ = require('lodash');
const Sequelize = require('sequelize')

module.exports = (sequelize, DataTypes) => {
  var Model = sequelize.define('Agent', {
    id: {
      type: DataTypes.STRING(32),
      primaryKey: true,
      defaultValue: sequelize.fn('generate_object_id'),
      allowNull: false,
    },
    type: {
      type: DataTypes.ENUM(["USER", "BOT"]),
      allowNull: false,
    },
    name: {
      allowNull: true,
      type: Sequelize.VIRTUAL(DataTypes.STRING),
      get: async function() {
        if (this.type == "USER"){
          const user = await this.getUser();
          return _.get(user, 'name');
        } else {
          const bot = await this.getBot();
          return _.get(bot, 'name');
        }
      }
    },
    measurementCount: {
      allowNull: true,
      type: Sequelize.VIRTUAL(DataTypes.INTEGER),
      get: async function() {
        const items = await this.getMeasurements()
        return items.length
      }
    },
  });
  Model.associate = function (models) {
    Model.User = Model.hasOne(models.User, {
      foreignKey: 'agentId',
    })
    Model.Bot = Model.hasOne(models.Bot, {
      foreignKey: 'agentId',
    })
    Model.Measurements = Model.hasMany(models.Measurement, {
      foreignKey: 'agentId',
      as: 'Measurements'
    })
    Model.Measurables = Model.hasMany(models.Measurable,
      {
        foreignKey: 'creatorId',
    }
    )
  }
  return Model;
};
