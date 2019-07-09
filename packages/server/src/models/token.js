module.exports = (sequelize, DataTypes) => {
  const Model = sequelize.define('Token', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: sequelize.fn('uuid_generate_v4'),
      allowNull: false,
    },
    agentId: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    token: {
      type: DataTypes.STRING,
      allowNull: true,
    },
    isActive: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      defaultValue: true,
    },
  });

  Model.associate = function associate(models) {
    Model.Agent = Model.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });
  };

  return Model;
};
