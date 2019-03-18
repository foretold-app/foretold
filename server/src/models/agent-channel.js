/**
 * No migrations needed.
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
