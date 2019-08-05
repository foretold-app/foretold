const { CHANNEL_MEMBERSHIP_ROLES } = require('./enums/channel-membership-roles');
const { CHANNEL_MEMBERSHIP_TYPE } = require('./enums/channel-membership-type');

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
      defaultValue: CHANNEL_MEMBERSHIP_ROLES.VIEWER,
    },
    methodCreatedBy: {
      type: DataTypes.ENUM([
        CHANNEL_MEMBERSHIP_TYPE.ADDED,
        CHANNEL_MEMBERSHIP_TYPE.JOINED,
        CHANNEL_MEMBERSHIP_TYPE.INVITED,
      ]),
      defaultValue: CHANNEL_MEMBERSHIP_TYPE.ADDED,
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

    models.ChannelMemberships.belongsTo(models.Agent, {
      foreignKey: 'inviterAgentId',
      as: 'inviterAgent',
    });
  };

  return ChannelMemberships;
};
