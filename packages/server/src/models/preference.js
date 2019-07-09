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
    emails: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      defaultValue: true,
    },
  });

  Preference.associate = function associate(models) {
    Preference.Agent = Preference.belongsTo(models.Agent, {
      foreignKey: 'agentId',
      as: 'agent',
    });
  };

  return Preference;
};
