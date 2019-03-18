'use strict';
const Sequelize = require('sequelize')

module.exports = (sequelize, DataTypes) => {
  var Model = sequelize.define('Series', {
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
    subjects: {
      type: DataTypes.ARRAY(DataTypes.STRING),
      allowNull: true,
    },
    properties: {
      type: DataTypes.ARRAY(DataTypes.STRING),
      allowNull: true,
    },
    dates: {
      type: DataTypes.ARRAY(DataTypes.DATE),
      allowNull: true,
    },
    // @deprecated
    channel: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    channelId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
  },
    {
    hooks: {
      afterCreate: async (series, options) => {
          await series.createMeasurables()
      }
    }
  });

  Model.prototype.createMeasurables = async function(){
    for (let subject of this.subjects){
        for (let property of this.properties){
            for (let date of this.dates){
                await sequelize.models.Measurable.create({
                    name: "",
                    descriptionEntity: subject,
                    descriptionProperty: property,
                    descriptionDate: date,
                    expectedResolutionDate: date,
                    seriesId: this.id,
                    creatorId: this.creatorId,
                    channel: this.channel,
                    valueType: "FLOAT"
                });
            }
        }
    }
  }

  Model.associate = function (models) {
    Model.Creator = Model.belongsTo(models.Agent, {
      foreignKey: 'creatorId',
      as: 'creator'
    })
    Model.Measurables = Model.hasMany(models.Measurable, {
      foreignKey: 'seriesId',
      as: "Measurables"
    })
    Model.Channel = Model.belongsTo(models.Channel, {
      foreignKey: 'channelId',
      as: 'channel'
    });
  }
  return Model;
};
