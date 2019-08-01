const { TOKEN_TYPE } = require('../src/models/enums/token-type');

const enumTypeName = 'enum_Tokens_type';
const newValue = TOKEN_TYPE.INVITE_TOKEN;

module.exports = {
  up: async function (queryInterface) {
    try {
      // ALTER TYPE ... ADD cannot run inside a transaction block
      await queryInterface.sequelize.query(
        `ALTER TYPE "${enumTypeName}" ` +
        `ADD VALUE '${newValue}'`
      );
    } catch (e) {
      console.error('Migration Up Error', e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`
        DELETE FROM pg_enum
        WHERE enumlabel = '${newValue}' AND enumtypid = (
            SELECT oid FROM pg_type WHERE typname = '${enumTypeName}'
        )
      `);
    } catch (e) {
      console.error('Migration Down Error', e);
      throw e;
    }
  }
};
