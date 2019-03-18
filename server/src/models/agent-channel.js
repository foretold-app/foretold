/**
 * No migrations needed.
 * @todo: Rename table to "agents_channels".
 * @param sequelize
 * @param DataTypes
 * @return {*}
 */
module.exports = (sequelize) => {
  const AgentChannel = sequelize.define('AgentChannel', {});

  AgentChannel.associate = function (models) {
    models.Agent.belongsToMany(models.Channel, { through: AgentChannel });
    models.Channel.belongsToMany(models.Agent, { through: AgentChannel });
  };

  return AgentChannel;
};
