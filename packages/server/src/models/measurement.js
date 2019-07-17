const _ = require('lodash');
const Sequelize = require('sequelize');
const { clientUrl } = require('../lib/urls');

const { MEASUREMENT_COMPETITOR_TYPE } = require('./enums/measurement-competitor-type');
const { MEASUREMENT_VALUE } = require('./enums/measurement-value');

module.exports = (sequelize, DataTypes) => {
  const Measurement = sequelize.define('Measurement', {
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
    valueText: {
      type: DataTypes.TEXT,
      allowNull: true,
    },
    competitorType: {
      type: DataTypes.ENUM([
        MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE,
        MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE,
        MEASUREMENT_COMPETITOR_TYPE.AGGREGATION,
        MEASUREMENT_COMPETITOR_TYPE.UNRESOLVED,
        MEASUREMENT_COMPETITOR_TYPE.COMMENT,
      ]),
      defaultValue: MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE,
      allowNull: true,
    },
    description: {
      type: DataTypes.TEXT,
      allowNull: true,
    },
    measurableId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    agentId: {
      type: DataTypes.UUID(),
      allowNull: true,
    },
    relevantAt: {
      allowNull: false,
      type: Sequelize.DATE,
      defaultValue: DataTypes.NOW,
    },
    taggedMeasurementId: {
      type: DataTypes.UUID(),
      allowNull: true,
    },
    createdAt: {
      type: DataTypes.DATE,
      defaultValue: DataTypes.NOW,
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: DataTypes.NOW,
    },
  });

  Measurement.addHook('beforeValidate', async (instance) => {
    if (instance.dataValues.relevantAt == null) {
      instance.relevantAt = Date.now();
    }
  });

  Measurement.addHook('afterCreate', async (measurement) => {
    const competitorType = measurement.dataValues.competitorType;
    const isJudgable = [
      MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE,
      MEASUREMENT_COMPETITOR_TYPE.UNRESOLVED,
    ].includes(competitorType);

    if (isJudgable) {
      const measurable = await measurement.getMeasurable();
      await measurable.judged();
    }
  });

  /**
   * @param {object} value
   */
  function setMeasurementValue(value) {
    let data, dataType;

    const types = Object.values(MEASUREMENT_VALUE);

    for (const type of types) {
      const valueOfType = _.get(value, type);
      const isEmpty = [null, undefined].includes(valueOfType);

      if (!isEmpty) {
        data = valueOfType;
        dataType = type;
      }
    }

    if (data !== undefined && dataType !== undefined) {
      this.dataValues.value = { data, dataType };
    }
  }

  /**
   * @return {{}|*}
   */
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
   * @return {Promise<*>}
   */
  Measurement.prototype.getCreationNotification =
    async function getCreationNotification(creator) {
      const agent = await creator.getAgent();
      const measurable = await this.getMeasurable();
      return {
        attachments: [{
          pretext: 'New Measurement Created',
          title: measurable.name,
          title_link: `${clientUrl}/c/${measurable.channelId}`,
          author_name: creator.name,
          author_link: `${clientUrl}/agents/${agent.id}`,
          text: this.description,
          fields: [
            {
              title: 'Type',
              value: this.competitorType,
              short: true,
            },
          ],
          color: '#d2ebff',
        }],
      };
    };

  Measurement.associate = function associate(models) {
    Measurement.Measurable = Measurement.belongsTo(models.Measurable, {
      foreignKey: 'measurableId',
    });

    Measurement.Agent = Measurement.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });

    Measurement.TaggedMeasurement = Measurement.belongsTo(models.Measurement, {
      foreignKey: 'taggedMeasurementId',
      as: 'TaggedMeasurement',
    });

    Measurement.TaggedBy = Measurement.hasMany(models.Measurement, {
      foreignKey: 'taggedMeasurementId',
      as: 'TaggedBy',
    });
  };

  return Measurement;
};
