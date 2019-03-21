const models = require('../src/models');

module.exports = {
  up: async function (queryInterface, Sequelize) {
    await models.User.findOrCreate({
      where: { name: 'Service Account' },
      defaults: {
        name: 'Service Account',
        auth0Id: Sequelize.fn('uuid_generate_v4')
      },
    });
  },

  down: async function () {
    await models.User.destroy({
      where: { name: 'Service Account' },
    });
  }
};
