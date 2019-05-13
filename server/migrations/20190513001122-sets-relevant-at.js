module.exports = {
  up: async function (queryInterface, Sequelize) {
    queryInterface.bulkUpdate('Measurements', {
      relevantAt: Sequelize.col('createdAt'),
    });
  },

  down: async function (queryInterface, Sequelize) {
    queryInterface.bulkUpdate('Measurements', {
      relevantAt: null,
    }, {
      taggedMeasurementId: {
        [Sequelize.Op.eq]: null,
      }
    });
  }
};
