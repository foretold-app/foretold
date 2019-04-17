module.exports = {
  up: async function (queryInterface, Sequelize) {
    await queryInterface.removeConstraint('Bots', 'Bots_agentId_fkey');
    await queryInterface.removeConstraint('Bots', 'Bots_userId_fkey');

    await queryInterface.renameColumn('Bots', 'userId', 'ownerUserId');
    await queryInterface.renameColumn('Bots', 'agentId', 'ownerAgentId');

    await queryInterface.addConstraint('Bots', ['ownerUserId'], {
      type: 'foreign key',
      name: 'Bots_ownerUserId_fkey',
      references: {
        table: 'Users',
        field: 'id'
      },
      onDelete: 'cascade',
      onUpdate: 'cascade'
    });

    await queryInterface.addConstraint('Bots', ['ownerAgentId'], {
      type: 'foreign key',
      name: 'Bots_ownerAgentId_fkey',
      references: {
        table: 'Agents',
        field: 'id'
      },
      onDelete: 'cascade',
      onUpdate: 'cascade'
    });

    await queryInterface.changeColumn('Bots', 'ownerUserId', {
      type: Sequelize.DataTypes.UUIDV4,
      allowNull: false,
    });
  },

  down: async function (queryInterface) {
    await queryInterface.removeConstraint('Bots', 'Bots_ownerUserId_fkey');
    await queryInterface.removeConstraint('Bots', 'Bots_ownerAgentId_fkey');

    await queryInterface.renameColumn('Bots', 'ownerUserId', 'userId');
    await queryInterface.renameColumn('Bots', 'ownerAgentId', 'agentId');

    await queryInterface.addConstraint('Bots', ['userId'], {
      type: 'foreign key',
      name: 'Bots_userId_fkey',
      references: {
        table: 'Users',
        field: 'id'
      },
      onDelete: 'cascade',
      onUpdate: 'cascade'
    });

    await queryInterface.addConstraint('Bots', ['agentId'], {
      type: 'foreign key',
      name: 'Bots_agentId_fkey',
      references: {
        table: 'Agents',
        field: 'id'
      },
      onDelete: 'cascade',
      onUpdate: 'cascade'
    });

    await queryInterface.changeColumn('Bots', 'userId', {
      type: Sequelize.DataTypes.UUIDV4,
      allowNull: true,
    });
  }
};
