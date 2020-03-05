const { CHANNEL_MEMBERSHIP_ROLES } = require('../../../enums');
const { CHANNEL_MEMBERSHIP_TYPE } = require('../../../enums');

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
      allowNull: false,
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
      allowNull: false,
    },
  });

  ChannelMemberships.ROLE = CHANNEL_MEMBERSHIP_ROLES;

  return ChannelMemberships;
};
