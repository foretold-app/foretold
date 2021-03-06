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
    totalVotesReceived: {
      type: DataTypes.INTEGER,
      allowNull: true,
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

  return AgentMeasurable;
};
