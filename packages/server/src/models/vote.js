// Do not use associations between models and do not
// use ORM for fetching them!
module.exports = (sequelize, DataTypes) => {
  const Vote = sequelize.define('Vote', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: DataTypes.UUIDV4,
      allowNull: false,
    },
    measurableId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    agentId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    voteAmount: {
      type: DataTypes.SMALLINT,
      allowNull: false,
      defaultValue: 0,
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
  return Vote;
};
