/**
 * @param sequelize
 * @return {*}
 */
module.exports = (sequelize) => {
  const AgentsChannels = sequelize.define('AgentsChannels', {});

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
