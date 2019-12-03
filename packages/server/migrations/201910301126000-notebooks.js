module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      await queryInterface.createTable('Notebooks', {
        id: {
          allowNull: false,
          primaryKey: true,
          type: Sequelize.UUID
        },
        name: {
          type: Sequelize.STRING(256),
          allowNull: false,
        },
        ownerId: {
          type: Sequelize.UUID,
          references: {
            model: 'Agents',
            key: 'id',
          },
          allowNull: false,
        },
        channelId: {
          type: Sequelize.UUID,
          references: {
            model: 'Channels',
            key: 'id',
          },
          allowNull: false,
        },
        body: {
          type: Sequelize.TEXT,
          allowNull: true,
        },
        createdAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
        },
        updatedAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
        },
      });

      await queryInterface.addIndex('Notebooks', ['name', 'ownerId'], {
        name: 'Notebooks_name_ownerId_unique',
        unique: true,
      });

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Up Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      await queryInterface.removeIndex(
        'Notebooks',
        'Notebooks_name_ownerId_unique',
      );
      await queryInterface.dropTable('Notebooks');
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  }
};
