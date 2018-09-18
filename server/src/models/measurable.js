'use strict';
const Sequelize = require('sequelize')

module.exports = (sequelize, DataTypes) => {
  var Model = sequelize.define('Measurable', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: Sequelize.UUIDV4,
      allowNull: false,
    },
    name: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    valueType: {
      type: DataTypes.ENUM(["FLOAT", "DATE", "PERCENTAGE"]),
      allowNull: false,
    },
    isLocked: {
      allowNull: false,
      defaultValue: false,
      type: DataTypes.BOOLEAN
    },
    lockedAt: {
      allowNull: true,
      type: DataTypes.DATE
    },
  });
  Model.associate = function (models) {
    Model.Measurements = Model.hasMany(models.Measurement, {
      foreignKey: 'measurableId',
      as: 'Measurements'
    })
  }
  return Model;
};
