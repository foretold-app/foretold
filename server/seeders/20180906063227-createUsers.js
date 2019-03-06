const models = require("../src/models");
import _ from "lodash";

const uuid = require('uuid/v4');

const users = [
  {
    name: "Goe Roggins"
  },
  {
    name: "Bill Holliday"
  },
  {
    name: "Dread Stein"
  },
  {
    name: "Paul Smith"
  }
];

const bots = [
  {
    name: "FooBot",
    competitorType: "COMPETITIVE",
  },
  {
    name: "Bot 2",
    competitorType: "COMPETITIVE",
  },
  {
    name: "Bot 3",
    competitorType: "COMPETITIVE",
  },
  {
    name: "Bot 4",
    competitorType: "AGGREGATION",
  }
]:

const measurables = [
  {
    name: "Rating of usefulness of X",
    valueType: "FLOAT"
  },
  {
    name: "Rating of usefulness of Y",
    valueType: "FLOAT"
  },
  {
    name: "Rating of usefulness of Bednets",
    valueType: "DATE"
  },
  {
    name: "Rating of usefulness of Malaria",
    valueType: "PERCENTAGE"
  },
  {
    name: "Rating of usefulness of Georgia",
    valueType: "FLOAT"
  }
]

module.exports = {
  up: async (queryInterface, Sequelize) => {

    try {
      const xxx = await models.User.create({
        id: "1010a04d-5307-4bd0-9246-000000000000",
        name: "initial",
      });

      const yyy = await models.Bot.create({
        id: "1010a04d-5307-4bd0-9246-000000000000",
        userId: "1010a04d-5307-4bd0-9246-000000000000",
        name: "aggbot1"
      });

      const zzz = await models.Agent.create({
        id: "1010a04d-5307-4bd0-9256-000000000000",
        type: 'USER',
      });

      let allUsers = [];
      for (let user of users) {
        const u = await models.User.create({
          ...user,
          id: uuid(),
        });
        allUsers = [...allUsers, u];
      }

      let allBots = []
      for (let bot of bots) {
        let bb = {...bot, userId: allUsers[0].dataValues.id, description: "sdf"}
        const b = await models.Bot.create(bb)
        allBots = [...allBots, b]
      }

      let allMeasurables = []
      for (let m of measurables) {
        const u = await models.Measurable.create({ ...m, creatorId: zzz.id });
        allMeasurables = [...allMeasurables, u];
      }

      let measurements = _.times(50, () => 0);
      for (let m of measurements) {
        let user = _.sample(allUsers)
        let agent = await user.getAgent()
        let measurable = _.sample(allMeasurables)
        const u = await models.Measurement.create({
          agentId: agent.id,
          measurableId: measurable.id,
          isAggregation: false,
          competitorType: "COMPETITIVE",
          value: {trio: {p25: 10.5, p50: 40.5, p75: 100.5 }}
        })
      }

      for (let m of measurements) {
        let bot = _.sample(allBots)
        let agent = await bot.getAgent()
        let measurable = _.sample(allMeasurables)
        const u = await models.Measurement.create({
          agentId: agent.id,
          measurableId: measurable.id,
          isAggregation: false,
          competitorType: "COMPETITIVE",
          value: {trio: {p25: 12.5, p50: 43.5, p75: 130.5 }}
        })
      }
    } catch (e) {
      console.error(e);
    }

  },

    down: async (queryInterface, Sequelize) => {
      /*
        Add reverting commands here.
        Return a promise to correctly handle asynchronicity.

        Example:
        return queryInterface.bulkDelete('Person', null, {});
      */
    }
};
