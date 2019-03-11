'use strict';
const Sequelize = require('sequelize')

module.exports = (sequelize, DataTypes) => {
  var Model = sequelize.define('Bot', {
    id: {
      type: DataTypes.STRING(32),
      primaryKey: true,
      defaultValue: sequelize.fn('generate_object_id'),
      allowNull: false,
    },
    name: {
      type: DataTypes.STRING,
      allowNull: true,
    },
    description: {
      type: DataTypes.STRING,
      allowNull: true,
    },
    competitorType: {
      type: DataTypes.ENUM(["COMPETITIVE", "AGGREGATION", "OBJECTIVE"]),
      defaultValue: "COMPETITIVE",
      allowNull: true,
    },
  },
  {
    hooks: {
      beforeCreate: async (event, options) => {
        let agent = await sequelize.models.Agent.create({
          type: "BOT",
        });
        event.agentId = agent.dataValues.id
      }
    }
  });
  Model.associate = function (models) {
    Model.User = Model.belongsTo(models.User, {
      foreignKey: 'userId',
    })
    Model.Agent = Model.belongsTo(models.Agent, {foreignKey: 'agentId'})
  }
  return Model;
};
