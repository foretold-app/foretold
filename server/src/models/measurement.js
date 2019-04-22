const _ = require('lodash');
const Sequelize = require('sequelize');
const { clientUrl } = require('../lib/urls');

module.exports = (sequelize, DataTypes) => {
  const Model = sequelize.define('Measurement', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: DataTypes.UUIDV4,
      allowNull: false,
    },
    value: {
      type: DataTypes.JSON,
      allowNull: false,
      get: getMeasurementValue,
      set: setMeasurementValue,
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
      type: DataTypes.UUID(),
      allowNull: false
    },
    agentId: {
      type: DataTypes.UUID(),
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
      afterCreate: async (measurement) => {
        if (measurement.dataValues.competitorType == "OBJECTIVE") {
          const measurable = await measurement.getMeasurable();
          await measurable.judged();
        }
      }
    }
  });

  /**
   * @param {object} value
   */
  function setMeasurementValue(value) {
    let data, dataType;

    const types = ['floatCdf', 'floatPoint'];
    for (const type of types) {
      if (_.has(value, type)) {
        data = _.get(value, type);
        dataType = type;
      }
    }

    if (data && dataType) {
      this.dataValues.value = { data, dataType };
    }
  }

  function getMeasurementValue() {
    const value = _.get(this, 'dataValues.value');
    const data = _.get(this, 'dataValues.value.data');
    const dataType = _.get(this, 'dataValues.value.dataType');
    if (dataType !== undefined && data !== undefined) {
      return { [dataType]: data };
    }
    return value;
  }

  /**
   * @todo: move me
   * @public
   * @param {Models.Creator} creator
   * @return {Promise<void>}
   */
  Model.prototype.creationNotification = async function creationNotification(creator) {
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
      }]
    };
    return notification;
  };

  Model.associate = function associate(models) {
    Model.Measurable = Model.belongsTo(models.Measurable, {
      foreignKey: 'measurableId'
    });

    Model.Agent = Model.belongsTo(models.Agent, {
      foreignKey: 'agentId'
    });

    Model.TaggedMeasurement = Model.belongsTo(models.Measurement, {
      foreignKey: 'taggedMeasurementId',
      as: 'TaggedMeasurement'
    });

    Model.TaggedBy = Model.hasMany(models.Measurement, {
      foreignKey: 'taggedMeasurementId',
      as: 'TaggedBy'
    })
  };

  return Model;
};
