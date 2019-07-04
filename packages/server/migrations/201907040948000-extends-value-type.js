const enumTypeName = 'enum_Measurables_valueType';
const newValue = 'UNRESOLVED';

module.exports = {
  up: (queryInterface) => {
    return queryInterface.sequelize.query(
      `ALTER TYPE "${enumTypeName}" ADD VALUE '${newValue}'`,
    );
  },

  down: (queryInterface) => {
    return queryInterface.sequelize.query(`
        DELETE FROM pg_enum
        WHERE enumlabel = '${newValue}' AND enumtypid = (
            SELECT oid FROM pg_type WHERE typname = '${enumTypeName}'
        )
    `);
  }
};
