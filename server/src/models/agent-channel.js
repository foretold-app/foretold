/**
 * @param sequelize
 * @param DataTypes
 * @return {*}
 */
module.exports = (sequelize, DataTypes) => {
  const AgentsChannels = sequelize.define('AgentsChannels', {
    roles: {
      type: DataTypes.ARRAY(DataTypes.STRING(32)),
      allowNull: false,
    },
  });

  AgentsChannels.ROLES = {
    admin: 'admin',
    viewer: 'viewer',
    owner: 'owner',
  };

  AgentsChannels.associate = function (models) {
    models.Agent.belongsToMany(models.Channel, {
      through: AgentsChannels,
      foreignKey: 'id',
      as: 'agentId',
    });
    models.Channel.belongsToMany(models.Agent, {
      through: AgentsChannels,
      foreignKey: 'id',
      as: 'channelId',
    });
  };

  return AgentsChannels;
};
