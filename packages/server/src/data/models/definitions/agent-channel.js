module.exports = (sequelize, DataTypes) => {
  const AgentChannel = sequelize.define('AgentChannel', {
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
    channelId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    primaryPointScore: {
      type: DataTypes.FLOAT,
      allowNull: false,
    },
    numberOfPredictions: {
      type: DataTypes.INTEGER,
      allowNull: false,
    },
    numberOfQuestionsScored: {
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

  return AgentChannel;
};
