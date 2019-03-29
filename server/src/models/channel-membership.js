/**
 * @param sequelize
 * @param DataTypes
 * @return {*}
 */
module.exports = (sequelize, DataTypes) => {
  const ChannelsMemberships = sequelize.define('ChannelsMemberships', {
    role: {
      type: DataTypes.STRING(8),
      allowNull: false,
    },
  });

  ChannelsMemberships.ROLE = {
    ADMIN: 'ADMIN',
    VIEWER: 'VIEWER',
  };

  ChannelsMemberships.associate = function (models) {
    models.Agent.belongsToMany(models.Channel, {
      through: ChannelsMemberships,
      foreignKey: 'id',
      as: 'agentId',
    });
    models.Channel.belongsToMany(models.Agent, {
      through: ChannelsMemberships,
      foreignKey: 'id',
      as: 'channelId',
    });
  };

  return ChannelsMemberships;
};
