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
    value: {
      type: DataTypes.JSON,
      allowNull: false
    },
    competitorType: {
      type: DataTypes.ENUM(["OBJECTIVE", "COMPETITIVE", "AGGREGATION"]),
      defaultValue: "COMPETITIVE",
      allowNull: true,
    },
    measurableId: {
      type: DataTypes.UUID,
      allowNull: false
    },
    agentId: {
      type: DataTypes.UUID,
      allowNull: true
    },
    relevantAt: {
      allowNull: true,
      type: Sequelize.DATE,
      get() {
        return this.getDataValue('relevantAt') || this.getDataValue('createdAt')
      }
    },
  });
  Model.associate = function (models) {
    Model.Measurable = Model.belongsTo(models.Measurable, {
      foreignKey: 'measurableId'
    })
    Model.Agent = Model.belongsTo(models.Agent, {
      foreignKey: 'agentId'
    })
    Model.TaggedMeasurement = Model.belongsTo(models.Measurement, {
      foreignKey: 'taggedMeasurementId',
      as: 'TaggedMeasurement'
    })
    Model.TaggedBy = Model.hasMany(models.Measurement, {
      foreignKey: 'taggedMeasurementId',
      as: 'TaggedBy'
    })
  }
  return Model;
};
