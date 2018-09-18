'use strict';
const Sequelize = require('sequelize')

module.exports = (sequelize, DataTypes) => {
  var Model = sequelize.define('Bot', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: Sequelize.UUIDV4,
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
        console.log(3)
        let agent = await sequelize.models.Agent.create({
          type: "BOT",
        });
        console.log(event)
        event.agentId = agent.dataValues.id
        console.log(8)
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
