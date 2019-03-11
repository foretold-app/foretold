module.exports = {
  /**
   * @param {QueryInterface} queryInterface
   * @param {sequelize.DataTypes} Sequelize
   */
  up: function (queryInterface) {
    return queryInterface.sequelize.query(
        '-- This adaptation is released under the MIT License.\n' +
        'CREATE EXTENSION IF NOT EXISTS pgcrypto;\n' +
        'CREATE SEQUENCE epoch_seq INCREMENT BY 1 MAXVALUE 9 CYCLE;\n' +
        'CREATE OR REPLACE FUNCTION generate_object_id() RETURNS varchar AS $$\n' +
        '    DECLARE\n' +
        '        time_component bigint;\n' +
        '        epoch_seq int;\n' +
        '        machine_id text := encode(gen_random_bytes(3), \'hex\');\n' +
        '        process_id bigint;\n' +
        '        seq_id text := encode(gen_random_bytes(3), \'hex\');\n' +
        '        result varchar:= \'\';\n' +
        '    BEGIN\n' +
        '        SELECT FLOOR(EXTRACT(EPOCH FROM clock_timestamp())) INTO time_component;\n' +
        '        SELECT nextval(\'epoch_seq\') INTO epoch_seq;\n' +
        '        SELECT pg_backend_pid() INTO process_id;\n' +
        '\n' +
        '        result := result || lpad(to_hex(time_component), 8, \'0\');\n' +
        '        result := result || machine_id;\n' +
        '        result := result || lpad(to_hex(process_id), 4, \'0\');\n' +
        '        result := result || seq_id;\n' +
        '        result := result || epoch_seq;\n' +
        '        RETURN result;\n' +
        '    END;\n' +
        '$$ LANGUAGE PLPGSQL;'
      );
  },

  down: async function (migration) {
    return migration.sequelize.query(`      
      DROP EXTENSION IF EXISTS pgcrypto;
      DROP SEQUENCE IF EXISTS epoch_seq;
      DROP FUNCTION IF EXISTS generate_object_id();
    `);
  }
};
