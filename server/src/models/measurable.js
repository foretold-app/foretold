'use strict';
const Sequelize = require('sequelize')
import _ from "lodash";
const fetch = require("node-fetch");
const moment = require('moment');
const {clientUrl} = require('../lib/urls');

const states = {
  ARCHIVED: "ARCHIVED",
  OPEN: "OPEN",
  JUDGED: "JUDGED"
}

module.exports = (sequelize, DataTypes) => {
  var Model = sequelize.define('Measurable', {
    id: {
      type: DataTypes.STRING(32),
      primaryKey: true,
      defaultValue: sequelize.fn('generate_object_id'),
      allowNull: false,
    },
    name: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    descriptionEntity: {
      type: DataTypes.STRING,
      allowNull: true,
    },
    descriptionDate: {
      type: DataTypes.DATE,
      allowNull: true,
    },
    descriptionProperty: {
      type: DataTypes.STRING,
      allowNull: true,
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
    isJudged: {
      allowNull: false,
      defaultValue: false,
      type: DataTypes.BOOLEAN
    },
    state: {
      type: DataTypes.STRING,
      defaultValue: "OPEN",
      allowNull: false,
    },
    channel: {
      type: DataTypes.STRING,
      defaultValue: "general",
      allowNull: true,
    },
    stateUpdatedAt: {
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
        if ( !endpoint ||endpoint.length == 0|| endpoint == "") {
          return false
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

  Model.prototype.updateState = async function(state){
      await this.update({state, stateUpdatedAt: new Date()})
  }

  Model.prototype.archive = async function(){
      await this.updateState(states.ARCHIVED)
  }

  Model.prototype.unarchive = async function(){
    await this.updateState(this.isJudged ? states.JUDGED : states.OPEN)
  }

  Model.prototype.judged = async function(){
    if (!this.isJudged || this.state !== states.JUDGED) {
      await this.update({isJudged: true})
      await this.updateState(states.JUDGED)
    }
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
      await this.update({hasResolutionEndpointResolved: true});
      await this.judged();
    }
  }

  Model.prototype.creationNotification= async function(creator){
    let agent = await creator.getAgent();
    let notification = {
      "attachments": [{
        "pretext": "New Measurable Created",
        "title": this.name,
        "title_link": `${clientUrl}/measurables/${this.id}`,
        "author_name": creator.name,
        "author_link": `${clientUrl}/agents/${agent.id}`,
        "text": this.description,
        "fields": [
            {
                "title": "Resolution Date",
                "value": moment(this.expectedResolutionDate).format("MMM DD, YYYY"),
                "short": true
            }
        ],
        "color": "#4a8ed8"
    }]};
    return notification;
  }

  Model.prototype.changedFields = function(ops){
    return Object.keys(ops).filter(r => r !== "expectedResolutionDate").filter(r => this[r] !== ops[r]);
  }

  Model.prototype.updateNotifications = async function(creator, newData){
    let changed = this.changedFields(newData);
    let agent = await creator.getAgent();
    let notification = {
      "attachments": [{
        "pretext": "Measurable Updated",
        "title": this.name,
        "title_link": `${clientUrl}/measurables/${this.id}`,
        "author_name": creator.name,
        "author_link": `${clientUrl}/agents/${agent.id}`,
        "fields": changed.map(c => ({
          "title": c,
          "short": false,
          "value": `*From*: ${this[c]} \n*To*:  ${newData[c]}`
        })),
        "color": "#ffe75e"
    }]};
    return notification;
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
