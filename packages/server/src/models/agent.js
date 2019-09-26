const _ = require('lodash');
const Sequelize = require('sequelize');

const { AGENT_TYPE } = require('../enums/agent-type');

module.exports = (sequelize, DataTypes) => {
  const Agent = sequelize.define('Agent', {
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
      get: getName,
    },
    measurementCount: {
      allowNull: true,
      type: Sequelize.VIRTUAL(DataTypes.INTEGER),
      get: getMeasurementCount,
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

  async function getName() {
    if (this.type === AGENT_TYPE.USER) {
      const user = await this.getUser();
      return _.get(user, 'name');
    }
    const bot = await this.getBot();
    return _.get(bot, 'name');
  }

  async function getMeasurementCount() {
    const items = await this.getMeasurements();
    return items.length;
  }

  Agent.associate = function associate(models) {
    Agent.User = Agent.hasOne(models.User, {
      foreignKey: 'agentId',
    });

    Agent.Bot = Agent.hasOne(models.Bot, {
      foreignKey: 'agentId',
    });

    Agent.Preference = Agent.hasOne(models.Preference, {
      foreignKey: 'agentId',
    });

    Agent.Measurements = Agent.hasMany(models.Measurement, {
      foreignKey: 'agentId',
      as: 'Measurements',
    });

    Agent.Measurables = Agent.hasMany(models.Measurable, {
      foreignKey: 'creatorId',
    });

    // await (await models.Agent.findByPk('4897a0f7-6b30-4ad3-a3d1-21c487a435ce')).getChannels()
    // models.Agent.findAll({ include: [models.Channel] })
    // await (await models.Agent.find({where: { id: '4897a0f7-6b30-4ad3-a3d1-21c487a435ce' } } )).getChannels();
    Agent.Channels = Agent.belongsToMany(models.Channel, {
      through: models.ChannelMemberships,
      foreignKey: 'agentId',
      otherKey: 'channelId',
    });
  };

  return Agent;
};
