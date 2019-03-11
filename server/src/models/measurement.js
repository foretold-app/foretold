'use strict';
const Sequelize = require('sequelize')
const {clientUrl} = require('../lib/urls');

module.exports = (sequelize, DataTypes) => {
  var Model = sequelize.define('Measurement', {
    id: {
      type: DataTypes.STRING(32),
      primaryKey: true,
      defaultValue: sequelize.fn('generate_object_id'),
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
    description: {
      type: DataTypes.TEXT,
      allowNull: true
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
  }, {
    hooks: {
      afterCreate: async (measurement, options) => {
        if (measurement.dataValues.competitorType == "OBJECTIVE"){
          const measurable = await measurement.getMeasurable();
          await measurable.judged();
        }
      }
    }
  });

  Model.prototype.creationNotification= async function(creator){
    let agent = await creator.getAgent();
    let measurable = await this.getMeasurable();
    let notification = await {
      "attachments": [{
        "pretext": "New Measurement Created",
        "title": measurable.name,
        "title_link": `${clientUrl}/measurables/${measurable.id}`,
        "author_name": creator.name,
        "author_link": `${clientUrl}/agents/${agent.id}`,
        "text": this.description,
        "fields": [
            {
                "title": "Type",
                "value": this.competitorType,
                "short": true
            }
        ],
        "color": "#d2ebff"
    }]};
    return notification;
  }
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
