module.exports = (sequelize, DataTypes) => {
  const Notebook = sequelize.define('Notebook', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: sequelize.fn('uuid_generate_v4'),
      allowNull: false,
    },
    // @entity: notebook-name-validation
    name: {
      type: DataTypes.STRING(255),
      allowNull: false,
      validate: {
        len: [3, 255],
      },
    },
    // @entity: notebook-body-validation
    body: {
      type: DataTypes.TEXT,
      allowNull: false,
      validate: {
        len: [3, 128 * 1024],
      },
    },
    channelId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    // @entity: creator-link
    ownerId: {
      type: DataTypes.UUID(),
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
  Notebook.associate = function associate(models) {
    Notebook.Channel = Notebook.belongsTo(models.Channel, {
      foreignKey: 'channelId',
    });
    Notebook.Agent = Notebook.belongsTo(models.Agent, {
      foreignKey: 'ownerId',
    });
  };

  return Notebook;
};
