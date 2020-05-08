const _ = require('lodash');

const models = require('../src/data/models/definitions');

/**
 * @todo: To check this file manually, does it work?
 * @todo: Last check date: _.
 */

const users = [
  {
    name: 'Goe Roggins',
  },
  {
    name: 'Bill Holliday',
  },
  {
    name: 'Dread Stein',
  },
  {
    name: 'Paul Smith',
  },
];

const bots = [
  {
    name: 'FooBot',
    competitorType: 'COMPETITIVE',
  },
  {
    name: 'Bot 2',
    competitorType: 'COMPETITIVE',
  },
  {
    name: 'Bot 3',
    competitorType: 'COMPETITIVE',
  },
  {
    name: 'Bot 4',
    competitorType: 'AGGREGATION',
  },
];

const measurables = [
  {
    name: 'Rating of usefulness of X',
    valueType: 'FLOAT',
  },
  {
    name: 'Rating of usefulness of Y',
    valueType: 'FLOAT',
  },
  {
    name: 'Rating of usefulness of Bednets',
    valueType: 'DATE',
  },
  {
    name: 'Rating of usefulness of Malaria',
    valueType: 'PERCENTAGE',
  },
  {
    name: 'Rating of usefulness of Georgia',
    valueType: 'FLOAT',
  },
];

module.exports = {
  up: async (_e) => {
    await models.User.create({
      id: '1010a04d-5307-4bd0-9246-000000000000',
      name: 'initial',
    });

    await models.Bot.create({
      id: '1010a04d-5307-4bd0-9246-000000000000',
      userId: '1010a04d-5307-4bd0-9246-000000000000',
      name: 'aggbot1',
    });

    let allUsers = [];
    for (let user of users) {
      const u = await models.User.create(user);
      allUsers = [...allUsers, u];
    }

    let allBots = [];
    for (let bot of bots) {
      const bb = {
        ...bot,
        userId: allUsers[0].id,
        description: 'sdf',
      };
      const b = await models.Bot.create(bb);
      allBots = [...allBots, b];
    }

    let allMeasurables = [];
    for (let m of measurables) {
      const u = await models.Measurable.create(m);
      allMeasurables = [...allMeasurables, u];
    }

    let measurements = _.times(50, () => 0);
    for (let m of measurements) {
      let user = _.sample(allUsers);
      let agent = await user.getAgent();
      let measurable = _.sample(allMeasurables);

      await models.Measurement.create({
        agentId: agent.id,
        measurableId: measurable.id,
        isAggregation: false,
        competitorType: 'COMPETITIVE',
        value: { trio: { p25: 10.5, p50: 40.5, p75: 100.5 } },
      });
    }

    for (let m of measurements) {
      let bot = _.sample(allBots);
      let agent = await bot.getAgent();
      let measurable = _.sample(allMeasurables);

      await models.Measurement.create({
        agentId: agent.id,
        measurableId: measurable.id,
        isAggregation: false,
        competitorType: 'COMPETITIVE',
        value: { trio: { p25: 12.5, p50: 43.5, p75: 130.5 } },
      });
    }
  },

  down: async () => {
  },
};
