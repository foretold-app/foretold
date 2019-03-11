module.exports = {
  /**
   * @param {QueryInterface} queryInterface
   * @param {sequelize.DataTypes} Sequelize
   */
  up: function (queryInterface) {
    return queryInterface.sequelize.query(`
        ALTER TABLE "Bots" DROP CONSTRAINT "Bots_userId_fkey";
        ALTER TABLE "Bots" DROP CONSTRAINT "Bots_agentId_fkey";
        
        ALTER TABLE "Measurements" DROP CONSTRAINT "Measurements_agentId_fkey";
        ALTER TABLE "Measurements" DROP CONSTRAINT "Measurements_measurableId_fkey";
        ALTER TABLE "Measurements" DROP CONSTRAINT "Measurements_taggedMeasurementId_fkey";
        
        ALTER TABLE "Users" DROP CONSTRAINT "Users_agentId_fkey";
        
        ALTER TABLE "Measurables" DROP CONSTRAINT "Measurables_creatorId_fkey";
        
        
        
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
        ALTER TABLE "Bots" ALTER COLUMN "agentId" TYPE varchar(36);
        ALTER TABLE "Bots" ADD CONSTRAINT "Bots_agentId_fkey" FOREIGN KEY ("agentId") REFERENCES "Agents" ("id") on update cascade on delete set null;
        
        ALTER TABLE "Measurements" ALTER COLUMN "agentId" TYPE varchar(36);
        ALTER TABLE "Measurements" ADD CONSTRAINT "Measurements_agentId_fkey" FOREIGN KEY ("agentId") REFERENCES "Agents" ("id") on update cascade on delete set null;
        ALTER TABLE "Measurements" ALTER COLUMN "measurableId" TYPE varchar(36);
        ALTER TABLE "Measurements" ADD CONSTRAINT "Measurements_measurableId_fkey" FOREIGN KEY ("measurableId") REFERENCES "Measurables" ("id") on update cascade on delete set null;
        ALTER TABLE "Measurements" ALTER COLUMN "taggedMeasurementId" TYPE varchar(36);
        ALTER TABLE "Measurements" ADD CONSTRAINT "Measurements_taggedMeasurementId_fkey" FOREIGN KEY ("taggedMeasurementId") REFERENCES "Measurements" ("id") on update cascade on delete set null;
        
        ALTER TABLE "Users" ALTER COLUMN "agentId" TYPE varchar(36);
        ALTER TABLE "Users" ADD CONSTRAINT "Users_agentId_fkey" FOREIGN KEY ("agentId") REFERENCES "Agents" ("id") on update cascade on delete set null;
        
        ALTER TABLE "Measurables" ALTER COLUMN "creatorId" TYPE varchar(36);
        ALTER TABLE "Measurables" ADD CONSTRAINT "Measurables_creatorId_fkey" FOREIGN KEY ("creatorId") REFERENCES "Agents" ("id") on update cascade on delete set null;
      `);
  },

  down: async function (migration) {
    return migration.sequelize.query(`      
      ALTER TABLE "Bots" DROP CONSTRAINT "Bots_userId_fkey";
      ALTER TABLE "Bots" ALTER COLUMN "userId" SET DATA TYPE uuid USING "userId"::uuid;
      ALTER TABLE "Bots" DROP CONSTRAINT "Bots_agentId_fkey";
      ALTER TABLE "Bots" ALTER COLUMN "agentId" SET DATA TYPE uuid USING "agentId"::uuid;
      
      ALTER TABLE "Measurements" DROP CONSTRAINT "Measurements_agentId_fkey";
      ALTER TABLE "Measurements" ALTER COLUMN "agentId" SET DATA TYPE uuid USING "agentId"::uuid;
      ALTER TABLE "Measurements" DROP CONSTRAINT "Measurements_measurableId_fkey";
      ALTER TABLE "Measurements" ALTER COLUMN "measurableId" SET DATA TYPE uuid USING "measurableId"::uuid;
      ALTER TABLE "Measurements" DROP CONSTRAINT "Measurements_taggedMeasurementId_fkey";
      ALTER TABLE "Measurements" ALTER COLUMN "taggedMeasurementId" SET DATA TYPE uuid USING "taggedMeasurementId"::uuid;
            
      ALTER TABLE "Users" DROP CONSTRAINT "Users_agentId_fkey";
      ALTER TABLE "Users" ALTER COLUMN "agentId" SET DATA TYPE uuid USING "agentId"::uuid;
      
      ALTER TABLE "Measurables" DROP CONSTRAINT "Measurables_creatorId_fkey";
      ALTER TABLE "Measurables" ALTER COLUMN "creatorId" SET DATA TYPE uuid USING "creatorId"::uuid;
      
      
      
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
      ALTER TABLE "Bots" ADD CONSTRAINT "Bots_agentId_fkey" FOREIGN KEY ("agentId") REFERENCES "Agents" ("id") on update cascade on delete set null;

      ALTER TABLE "Measurements" ADD CONSTRAINT "Measurements_agentId_fkey" FOREIGN KEY ("agentId") REFERENCES "Agents" ("id") on update cascade on delete set null;
      ALTER TABLE "Measurements" ADD CONSTRAINT "Measurements_measurableId_fkey" FOREIGN KEY ("measurableId") REFERENCES "Measurables" ("id") on update cascade on delete set null;
      ALTER TABLE "Measurements" ADD CONSTRAINT "Measurements_taggedMeasurementId_fkey" FOREIGN KEY ("taggedMeasurementId") REFERENCES "Measurements" ("id") on update cascade on delete set null;
      
      ALTER TABLE "Users" ADD CONSTRAINT "Users_agentId_fkey" FOREIGN KEY ("agentId") REFERENCES "Agents" ("id") on update cascade on delete set null;
      
      ALTER TABLE "Measurables" ADD CONSTRAINT "Measurables_creatorId_fkey" FOREIGN KEY ("creatorId") REFERENCES "Agents" ("id") on update cascade on delete set null;
    `);
  }
};
