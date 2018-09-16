'use strict';
const Sequelize = require('sequelize')

module.exports = (sequelize, DataTypes) => {
  var Model = sequelize.define('Measurement', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: Sequelize.UUIDV4,
      allowNull: false,
    },
    percentile25: {
      type: DataTypes.FLOAT,
      allowNull: false,
    },
    percentile50: {
      type: DataTypes.FLOAT,
      allowNull: false,
    },
    percentile75: {
      type: DataTypes.FLOAT,
      allowNull: false,
    },
    measurableId: {
      type: DataTypes.UUID,
      allowNull: false
    },
    agentId: {
      type: DataTypes.UUID,
      allowNull: true
    },
    derivedById: {
      type: DataTypes.UUID,
      allowNull: true
    }
  });
  Model.associate = function (models) {
    Model.Measurable = Model.belongsTo(models.Measurable, {
      foreignKey: 'measurableId'
    })
    Model.Agent = Model.belongsTo(models.Agent, {
      foreignKey: 'agentId'
    })
  }
  return Model;
};
