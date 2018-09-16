'use strict';
const models = require("../src/models")
import _ from "lodash"
import { getArgumentValues } from "graphql/execution/values";

const users = [{
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
]

const bots = [{
    name: "FooBot",
  },
  {
    name: "Bot 2",
  },
  {
    name: "Bot 3",
  },
  {
    name: "Bot 4",
  }
]

const measurables = [{
    name: "Rating of usefulness of X"
  },
  {
    name: "Rating of usefulness of Y"
  },
  {
    name: "Rating of usefulness of Bednets"
  },
  {
    name: "Rating of usefulness of Malaria"
  },
  {
    name: "Rating of usefulness of Georgia"
  }
]

module.exports = {
  up: async (queryInterface, Sequelize) => {
      let allUsers = []
      for (let user of users) {
        const u = await models.User.create(user)
        allUsers = [...allUsers, u]
      }

      let allBots = []
      for (let bot of bots) {
        let bb = {...bot, userId: allUsers[0].dataValues.id, description: "sdf"}
        const b = await models.Bot.create(bb)
        allBots = [...allBots, b]
      }

      let allMeasurables = []
      for (let m of measurables) {
        const u = await models.Measurable.create(m)
        allMeasurables = [...allMeasurables, u]
      }

      let measurements = _.times(50, () => 0);
      for (let m of measurements) {
        let user = _.sample(allUsers)
        let agent = await user.getAgent()
        let measurable = _.sample(allMeasurables)
        const u = await models.Measurement.create({
          agentId: agent.id,
          measurableId: measurable.id,
          percentile25: 2.0,
          percentile50: 5.0,
          percentile75: 8.0,
          isAggregation: false,
        })
      }

      for (let m of measurements) {
        let bot = _.sample(allBots)
        let agent = await bot.getAgent()
        let measurable = _.sample(allMeasurables)
        const u = await models.Measurement.create({
          agentId: agent.id,
          measurableId: measurable.id,
          percentile25: 2.0,
          percentile50: 5.0,
          percentile75: 8.0,
          isAggregation: false,
        })
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
