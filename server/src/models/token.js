module.exports = (sequelize, DataTypes) => {
  const Model = sequelize.define('Token', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: Sequelize.fn('uuid_generate_v4'),
      allowNull: false,
    },
    token: {
      type: DataTypes.STRING,
      allowNull: true,
    },
    agentId: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    issuedAtBefore: {
      type: DataTypes.DATE,
      allowNull: true,
    },
    issuedAtAfter: {
      type: DataTypes.DATE,
      allowNull: true,
    },
    isLocked: {
      type: DataTypes.BOOLEAN,
      allowNull: false,
      defaultValue: false,
    },
  });

  Model.associate = function (models) {
    Model.Agent = Model.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });
  };

  return Model;
};
