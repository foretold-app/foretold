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
      )
      .then(() => queryInterface.sequelize.query(`
        ALTER TABLE "Bots" DROP CONSTRAINT "Bots_userId_fkey";
        
        ALTER TABLE "Users" ALTER COLUMN id TYPE varchar(36);
        ALTER TABLE "Users" ALTER COLUMN id SET DEFAULT generate_object_id()::VARCHAR(24);
        
        ALTER TABLE "Agents" ALTER COLUMN id TYPE varchar(36);
        ALTER TABLE "Agents" ALTER COLUMN id SET DEFAULT generate_object_id()::VARCHAR(24);
        
        ALTER TABLE "Measurables" ALTER COLUMN id TYPE varchar(36);
        ALTER TABLE "Measurables" ALTER COLUMN id SET DEFAULT generate_object_id()::VARCHAR(24);
        
        ALTER TABLE "Bots" ALTER COLUMN id TYPE varchar(36);
        ALTER TABLE "Bots" ALTER COLUMN id SET DEFAULT generate_object_id()::VARCHAR(24);
        
        ALTER TABLE "Measurements" ALTER COLUMN id TYPE varchar(36);
        ALTER TABLE "Measurements" ALTER COLUMN id SET DEFAULT generate_object_id()::VARCHAR(24);
        
        ALTER TABLE "Bots" ALTER COLUMN "userId" TYPE varchar(36);
        ALTER TABLE "Bots" ADD CONSTRAINT "Bots_userId_fkey" FOREIGN KEY ("userId") REFERENCES "Users" ("id") on update cascade on delete set null;
      `));
  },

  down: async function (migration) {
    return migration.sequelize.query(`      
      ALTER TABLE "Bots" DROP CONSTRAINT "Bots_userId_fkey";
      ALTER TABLE "Bots" ALTER COLUMN "userId" SET DATA TYPE uuid USING "userId"::uuid;
      
      ALTER TABLE "Users" ALTER COLUMN id drop default;
      DELETE FROM "Users" WHERE length(id) = 24;
      ALTER TABLE "Users" ALTER COLUMN id TYPE uuid USING "id"::uuid;
      
      ALTER TABLE "Agents" ALTER COLUMN id drop default;
      DELETE FROM "Agents" WHERE length(id) = 24;
      ALTER TABLE "Agents" ALTER COLUMN id TYPE uuid USING "id"::uuid;
      
      ALTER TABLE "Measurables" ALTER COLUMN id drop default;
      DELETE FROM "Measurables" WHERE length(id) = 24;
      ALTER TABLE "Measurables" ALTER COLUMN id TYPE uuid USING "id"::uuid;
      
      ALTER TABLE "Bots" ALTER COLUMN id drop default;
      DELETE FROM "Bots" WHERE length(id) = 24;
      ALTER TABLE "Bots" ALTER COLUMN id TYPE uuid USING "id"::uuid;
      
      ALTER TABLE "Measurements" ALTER COLUMN id drop default;
      DELETE FROM "Measurements" WHERE length(id) = 24;
      ALTER TABLE "Measurements" ALTER COLUMN id TYPE uuid USING "id"::uuid;
      
      ALTER TABLE "Bots" ADD CONSTRAINT "Bots_userId_fkey" FOREIGN KEY ("userId") REFERENCES "Users" ("id") on update cascade on delete set null;
      
      DROP EXTENSION IF EXISTS pgcrypto;
      DROP SEQUENCE IF EXISTS epoch_seq;
      DROP FUNCTION IF EXISTS generate_object_id();
    `);
  }
};
