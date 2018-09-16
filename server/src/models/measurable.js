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
    }
  });
  Model.associate = function (models) {
    Model.Measurements = Model.hasMany(models.Measurement, {
      foreignKey: 'measurableId',
      as: 'Measurements'
    })
  }
  return Model;
};
