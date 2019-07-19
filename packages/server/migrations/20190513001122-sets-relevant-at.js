module.exports = {
  up: async function (queryInterface, Sequelize) {
    await queryInterface.sequelize.query(`BEGIN`);
    await queryInterface.bulkUpdate('Measurements', {
      relevantAt: Sequelize.col('createdAt'),
    });
    await queryInterface.sequelize.query(`COMMIT`);
  },

  down: async function (queryInterface, Sequelize) {
    await queryInterface.sequelize.query(`BEGIN`);
    await queryInterface.bulkUpdate('Measurements', {
      relevantAt: null,
    }, {
      taggedMeasurementId: {
        [Sequelize.Op.eq]: null,
      }
    });
    await queryInterface.sequelize.query(`COMMIT`);
  }
};
