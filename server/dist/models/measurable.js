'use strict';

var _lodash = require('lodash');

var _lodash2 = _interopRequireDefault(_lodash);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

const Sequelize = require('sequelize');


module.exports = (sequelize, DataTypes) => {
  var Model = sequelize.define('Measurable', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: Sequelize.UUIDV4,
      allowNull: false
    },
    name: {
      type: DataTypes.STRING,
      allowNull: false
    },
    description: {
      type: DataTypes.TEXT,
      allowNull: true
    },
    valueType: {
      type: DataTypes.ENUM(["FLOAT", "DATE", "PERCENTAGE"]),
      allowNull: false
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
      get: async function () {
        const items = await this.getMeasurements();
        return items.length;
      }
    },
    measurerCount: {
      allowNull: true,
      type: Sequelize.VIRTUAL(DataTypes.INTEGER),
      get: async function () {
        const items = await this.getMeasurements();
        return _lodash2.default.uniq(items.map(i => i.agentId)).length;
      }
    }
  });
  Model.associate = function (models) {
    Model.Measurements = Model.hasMany(models.Measurement, {
      foreignKey: 'measurableId',
      as: 'Measurements'
    });
    Model.Creator = Model.belongsTo(models.Agent, {
      foreignKey: 'creatorId',
      as: 'creator'
    });
  };
  return Model;
};
//# sourceMappingURL=measurable.js.map