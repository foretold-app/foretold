const moment = require('moment');
const util = require('util');
const _ = require('lodash');
const assert = require('assert');

const models = require('./models');

const uuid = () => 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, (letter, r) => {
  return ('x' === letter ? (r = Math.random() * 16 | 0) : (r & 0x3 | 0x8)).toString(16)
});

async function test() {
  await models.sequelize.query('BEGIN');

  const user1 = await models.User.create({
    name: 'Test User 1',
    auth0Id: uuid(),
  });

  const user2 = await models.User.create({
    name: 'Test User 2',
    auth0Id: uuid(),
  });
  const user3 = await models.User.create({
    name: 'Test User 3',
    auth0Id: uuid(),
  });

  const channel1 = await models.Channel.create({
    name: 'Test Channel 1',
    creatorId: user1.agentId,
  });
  await models.Channel.Agents.set(channel1, user1.agentId);

  const channel2 = await models.Channel.create({
    name: 'Test Channel 2',
    creatorId: user2.agentId,
  });
  await models.Channel.Agents.set(channel2, user2.agentId);

  const channel3 = await models.Channel.create({
    name: 'Test Channel 3',
    creatorId: user3.agentId,
  });
  await models.Channel.Agents.set(channel3, user3.agentId);

  const serie1 = await models.Series.create({
    name: 'Serie Test 1',
    channelId: channel1.id,
    creatorId: user1.agentId,
    subjects: ['A', 'B', 'C'],
    properties: ['AA', 'BB', 'CC'],
    dates: [moment().toDate(), moment().add(1, 'days').toDate(), moment().add(2, 'days').toDate()],
  });

  const measurable1 = await models.Measurable.create({
    name: 'Measurable Test 1',
    valueType: 'FLOAT',
    creatorId: user1.agentId,
    channelId: channel1.id,
  });

  const measurement1 = await models.Measurement.create({
    name: 'Measurement Test 1',
    measurableId: measurable1.id,
    competitorType: 'COMPETITIVE',
    value: 1,
    agentId: user1.agentId,
  });

  // 1
  {
    const ch = await models.Channel.findById(channel1.id);
    const cr = await ch.getCreator();

    console.log(cr.id, user1.agentId);
    assert(cr.id === user1.agentId, 'Error 1');
  }

  // 2
  {
    const ch = await models.Channel.findById(channel2.id);
    const ag = await ch.getAgents();
    assert(ag[0].id === user2.agentId, 'Error 2');
  }

  // 3
  {
    const ch = await (await models.Agent.findById(user3.agentId)).getChannels();
    assert(ch[0].id === channel3.id, 'Error 3');
  }

  await models.sequelize.query('ROLLBACK');

}

test().then((result) => {
  console.log('Test result', result);
}).catch((err) => {
  console.log('Tes err', err);
});
