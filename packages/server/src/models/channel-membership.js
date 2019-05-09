const { CHANNEL_MEMBERSHIP_ROLES } = require('./channel-membership-roles');

/**
 * @param sequelize
 * @param DataTypes
 * @return {*}
 */
module.exports = (sequelize, DataTypes) => {
  const ChannelMemberships = sequelize.define('ChannelMemberships', {
    role: {
      type: DataTypes.STRING(8),
      allowNull: false,
    },
  });

  ChannelMemberships.ROLE = CHANNEL_MEMBERSHIP_ROLES;

  ChannelMemberships.associate = function (models) {
    models.Agent.belongsToMany(models.Channel, {
      through: ChannelMemberships,
      foreignKey: 'id',
      as: 'agentId',
    });

    models.Channel.belongsToMany(models.Agent, {
      through: ChannelMemberships,
      foreignKey: 'id',
      as: 'channelId',
    });
  };

  return ChannelMemberships;
};
