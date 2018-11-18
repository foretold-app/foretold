'use strict';
const Sequelize = require('sequelize')
import _ from "lodash";
const fetch = require("node-fetch");

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
    description: {
      type: DataTypes.TEXT,
      allowNull: true,
    },
    resolutionEndpoint: {
      type: DataTypes.TEXT,
      allowNull: true,
    },
    hasResolutionEndpointResolved: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      defaultValue: false,
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
    expectedResolutionDate: {
      allowNull: true,
      type: DataTypes.DATE
    },
    measurementCount: {
      allowNull: true,
      type: Sequelize.VIRTUAL(DataTypes.INTEGER),
      get: async function() {
        const items = await this.getMeasurements()
        return items.length
      }
    },
    measurerCount: {
      allowNull: true,
      type: Sequelize.VIRTUAL(DataTypes.INTEGER),
      get: async function() {
        const items = await this.getMeasurements()
        return _.uniq(items.map(i => i.agentId)).length
      }
    },
    resolutionEndpointResponse: {
      allowNull: true,
      type: Sequelize.VIRTUAL(DataTypes.FLOAT),
      get: async function() {
        const endpoint = await this.dataValues.resolutionEndpoint;
        if (endpoint.length == 0 || !endpoint) {
          return null
        }
        try {
        const response = await fetch(endpoint);
        const json = await response.json();
        const match = JSON.stringify(json).match(/[-+]?[0-9]*\.?[0-9]+/);
        const asFloat = parseFloat(match[0]);
        console.log(`got response from endpoint. Url: ${endpoint}, Response: ${JSON.stringify(json)}, Float: ${asFloat}`);
        return asFloat;
        }
        catch(e) {
          console.error(`Error getting response from endpoint. Url: ${endpoint}, error: ${e}`)
          return null
        }
      }
    },
  });

  Model.needsResolutionResponse = async function(){
    return await Model.findAll({
      where: {
        hasResolutionEndpointResolved: false,
        resolutionEndpoint: {
          [Sequelize.Op.ne]: ""
        },
        expectedResolutionDate: {
          [Sequelize.Op.lt]: new Date()
        }
      }
    })
  }

  Model.prototype.lock = async function(){
      await this.update({isLocked: true, lockedAt: new Date()})
  }

  Model.prototype.processResolution = async function(agentId){
    const asFloat = await this.resolutionEndpointResponse;
    if (asFloat){
      await sequelize.models.Measurement.create({
          agentId,
          competitorType: "OBJECTIVE",
          measurableId: this.dataValues.id,
          value: {"dataType":"floatPoint","data":asFloat}
      });
      await this.update({hasResolutionEndpointResolved: true, isLocked: true})
    }
  }

  Model.associate = function (models) {
    Model.Measurements = Model.hasMany(models.Measurement, {
      foreignKey: 'measurableId',
      as: 'Measurements'
    })
    Model.Creator = Model.belongsTo(models.Agent, {
      foreignKey: 'creatorId',
      as: 'creator'
    })
  }
  return Model;
};
