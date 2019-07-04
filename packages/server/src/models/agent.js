const _ = require('lodash');
const Sequelize = require('sequelize');

const { AGENT_TYPE } = require('./enums/agent-type');

module.exports = (sequelize, DataTypes) => {
  const Model = sequelize.define('Agent', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: DataTypes.UUIDV4,
      allowNull: false,
    },
    type: {
      type: DataTypes.ENUM([
        AGENT_TYPE.USER,
        AGENT_TYPE.BOT,
      ]),
      allowNull: false,
    },
    isAdmin: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      defaultValue: false,
    },
    name: {
      allowNull: true,
      type: Sequelize.VIRTUAL(DataTypes.STRING),
      get: async function () {
        if (this.type === AGENT_TYPE.USER) {
          const user = await this.getUser();
          return _.get(user, 'name');
        } else {
          const bot = await this.getBot();
          return _.get(bot, 'name');
        }
      },
    },
    measurementCount: {
      allowNull: true,
      type: Sequelize.VIRTUAL(DataTypes.INTEGER),
      get: async function () {
        const items = await this.getMeasurements();
        return items.length;
      },
    },
  });

  Model.associate = function (models) {
    Model.User = Model.hasOne(models.User, {
      foreignKey: 'agentId',
    });

    Model.Bot = Model.hasOne(models.Bot, {
      foreignKey: 'agentId',
    });

    Model.Measurements = Model.hasMany(models.Measurement, {
      foreignKey: 'agentId',
      as: 'Measurements',
    });

    Model.Measurables = Model.hasMany(models.Measurable, {
      foreignKey: 'creatorId',
    });

    // await (await models.Agent.findByPk('4897a0f7-6b30-4ad3-a3d1-21c487a435ce')).getChannels()
    // models.Agent.findAll({ include: [models.Channel] })
    // await (await models.Agent.find({where: { id: '4897a0f7-6b30-4ad3-a3d1-21c487a435ce' } } )).getChannels();
    Model.Channels = Model.belongsToMany(models.Channel, {
      through: models.ChannelMemberships,
      foreignKey: 'agentId',
      otherKey: 'channelId',
    });
  };

  return Model;
};
