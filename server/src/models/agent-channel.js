/**
 * @param sequelize
 * @param DataTypes
 * @return {*}
 */
module.exports = (sequelize) => {
  const AgentChannel = sequelize.define('AgentsChannels', {}, {
    timestamps: false
  });

  AgentChannel.associate = function (models) {
    models.Agent.belongsToMany(models.Channel, {
      through: AgentChannel,
      foreignKey: 'id',
      as: 'agentId',
    });
    models.Channel.belongsToMany(models.Agent, {
      through: AgentChannel,
      foreignKey: 'id',
      as: 'channelId',
    });
  };

  return AgentChannel;
};
