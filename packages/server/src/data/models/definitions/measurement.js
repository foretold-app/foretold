const _ = require('lodash');

const { MEASUREMENT_COMPETITOR_TYPE } = require('../../../enums');
const { MEASUREMENT_VALUE } = require('../../../enums');

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
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
    },
    taggedMeasurementId: {
      type: DataTypes.UUID(),
      allowNull: true,
    },
    cancelledAt: {
      type: DataTypes.DATE,
      allowNull: true,
    },
    createdAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
      allowNull: false,
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
      allowNull: false,
    },
  });

  /**
   * @todo: To rework (to remove).
   * @param {object} value
   */
  function setMeasurementValue(value) {
    let data;
    let dataType;

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
      this.dataValues.value = {
        data,
        dataType,
      };
    }
  }

  /**
   * @todo: To rework (to remove).
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
   * @todo: fix it, remove it!
   * @deprecated
   * @param models
   */
  Measurement.associate = function associate(models) {
    Measurement.Measurable = Measurement.belongsTo(models.Measurable, {
      foreignKey: 'measurableId',
    });

    Measurement.Agent = Measurement.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });
  };

  return Measurement;
};
