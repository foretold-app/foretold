const { CHANNEL_MEMBERSHIP_ROLES } = require('./enums/channel-membership-roles');

/**
 * @param sequelize
 * @param DataTypes
 * @return {*}
 */
module.exports = (sequelize, DataTypes) => {
  const ChannelMemberships = sequelize.define('ChannelMemberships', {
    channelId: {
      type: DataTypes.UUID(),
      allowNull: false,
      primaryKey: true,
    },
    agentId: {
      type: DataTypes.UUID(),
      allowNull: false,
      primaryKey: true,
    },
    inviterAgentId: {
      type: DataTypes.UUID(),
      allowNull: true,
    },
    role: {
      type: DataTypes.STRING(8),
      allowNull: false,
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

  ChannelMemberships.ROLE = CHANNEL_MEMBERSHIP_ROLES;

  ChannelMemberships.associate = function associate(models) {
    models.Agent.belongsToMany(models.Channel, {
      through: ChannelMemberships,
      foreignKey: 'agentId',
      as: 'agentId',
    });

    models.Channel.belongsToMany(models.Agent, {
      through: ChannelMemberships,
      foreignKey: 'channelId',
      as: 'channelId',
    });

    models.Channel.hasOne(models.Agent, {
      foreignKey: 'inviterAgentId',
      as: 'inviterAgentId',
    });
  };

  return ChannelMemberships;
};
