module.exports = (sequelize, DataTypes) => {
  const Preference = sequelize.define('Preference', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: DataTypes.UUIDV4,
      allowNull: false,
    },
    agentId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    stopAllEmails: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      defaultValue: false,
    },
    enableExperimentalFeatures: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      defaultValue: false,
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
  Preference.associate = function associate(models) {
    Preference.Agent = Preference.belongsTo(models.Agent, {
      foreignKey: 'agentId',
      as: 'agent',
    });
  };

  return Preference;
};
