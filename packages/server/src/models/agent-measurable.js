module.exports = (sequelize, DataTypes) => {
  const AgentMeasurable = sequelize.define('AgentMeasurable', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: sequelize.fn('uuid_generate_v4'),
      allowNull: false,
    },
    agentId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    measurableId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    primaryPointScore: {
      type: DataTypes.FLOAT,
      allowNull: false,
    },
    timeAverageScore: {
      type: DataTypes.FLOAT,
      allowNull: false,
    },
    predictionCountTotal: {
      type: DataTypes.INTEGER,
      allowNull: false,
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

  /**
   * @todo: fix it, remove it.
   * @deprecated
   * @param models
   */
  AgentMeasurable.associate = function associate(models) {
    AgentMeasurable.Agent = AgentMeasurable.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });
    AgentMeasurable.Measurable = AgentMeasurable.belongsTo(models.Measurable, {
      foreignKey: 'measurableId',
    });
  };

  return AgentMeasurable;
};
