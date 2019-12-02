const { CHANNEL_MEMBERSHIP_ROLES } = require('../enums');
const { CHANNEL_MEMBERSHIP_TYPE } = require('../enums');

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
        CHANNEL_MEMBERSHIP_TYPE.ADDED_IN_APP_BY_ADMIN,
        CHANNEL_MEMBERSHIP_TYPE.AGENT_JOINED_DIRECTLY,
        CHANNEL_MEMBERSHIP_TYPE.ADDED_BY_EMAIL_BY_ADMIN,
      ]),
      defaultValue: CHANNEL_MEMBERSHIP_TYPE.ADDED_IN_APP_BY_ADMIN,
    },
    createdAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
    },
  });

  ChannelMemberships.ROLE = CHANNEL_MEMBERSHIP_ROLES;

  /**
   * @todo: fix it, remove it.
   * @deprecated
   * @param models
   */
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
