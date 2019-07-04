const Sequelize = require('sequelize');

const { MEASURABLE_VALUE_TYPE } = require('./measurable-value-type');

module.exports = (sequelize, DataTypes) => {
  const Model = sequelize.define('Series', {
      id: {
        type: DataTypes.UUID(),
        primaryKey: true,
        defaultValue: DataTypes.UUIDV4,
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
      channelId: {
        type: DataTypes.UUID(),
        allowNull: false,
      },
      measurableCount: {
        allowNull: true,
        type: Sequelize.VIRTUAL(DataTypes.INTEGER),
        get: async function () {
          // TODO: These queries are likely very slow,
          //  my guess is that this could be sped up a location.
          const items = await this.getMeasurables();
          return items.length;
        },
      },
    },
    {
      hooks: {
        afterCreate: async (series) => {
          await series.createMeasurables();
        },
      }
    });

  Model.prototype.createMeasurables = async function () {
    for (let subject of this.subjects) {
      for (let property of this.properties) {
        for (let date of this.dates) {
          console.log("Making Measurable for Series:", subject, property, date);
          await sequelize.models.Measurable.create({
            name: "",
            labelSubject: subject,
            labelProperty: property,
            labelOnDate: date,
            expectedResolutionDate: date,
            seriesId: this.id,
            creatorId: this.creatorId,
            channelId: this.channelId,
            valueType: MEASURABLE_VALUE_TYPE.FLOAT,
          });
        }
      }
    }
  };

  Model.associate = function (models) {
    Model.Creator = Model.belongsTo(models.Agent, {
      foreignKey: 'creatorId',
      as: 'creator',
    });

    Model.Measurables = Model.hasMany(models.Measurable, {
      foreignKey: 'seriesId',
      as: "Measurables",
    });

    // Usage
    // const se = await models.Series.find();
    // const ch = await se.getChannel();
    Model.Channel = Model.belongsTo(models.Channel, {
      foreignKey: 'channelId',
    });
  };

  return Model;
};
