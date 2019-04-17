module.exports = {
  up: async function (queryInterface) {
    // Warning
    await queryInterface.bulkDelete('Bots', {
      where: { userId: null },
    });
  },

  down: async function (queryInterface) {

  }
};
