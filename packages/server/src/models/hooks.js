const events = require('../async/events');
const emitter = require('../async/emitter');

const { MEASURABLE_STATE } = require('./enums/measurable-state');
const { AGENT_TYPE } = require('./enums/agent-type');
const { MEASUREMENT_COMPETITOR_TYPE } = require('./enums/measurement-competitor-type');

function addHooks(db) {
  db.sequelize.addHook('afterUpdate', (instance) => {
    if (
      instance instanceof db.Measurable &&
      instance.changed('state')
    ) {
      emitter.emit(events.MEASURABLE_STATE_IS_CHANGED, instance);
    }

    if (
      instance instanceof db.Measurable &&
      instance.changed('state') &&
      instance.get('state') === MEASURABLE_STATE.JUDGED
    ) {
      emitter.emit(events.MEASURABLE_STATE_IS_RESOLVED, instance);
    }
  });

  db.sequelize.addHook('afterCreate', (instance) => {
    if (instance instanceof db.ChannelMemberships) {
      emitter.emit(events.MEMBER_ADDED_TO_COMMUNITY, instance);
    }
  });

  db.Bot.addHook('beforeCreate', async (event) => {
    const agent = await db.sequelize.models.Agent.create({
      type: AGENT_TYPE.BOT,
    });
    event.agentId = agent.dataValues.id;
  });

  db.User.addHook('beforeCreate', async (event) => {
    const agent = await db.sequelize.models.Agent.create({
      type: AGENT_TYPE.USER,
    });
    event.agentId = agent.dataValues.id
  });

  db.Series.addHook('afterCreate', async (series) => {
    await series.createMeasurables();
  });

  db.Measurable.addHook('beforeUpdate', async (instance) => {
    await instance.watchExpectedResolutionDate(instance);
  });


  db.Measurement.addHook('beforeValidate', async (instance) => {
    if (instance.dataValues.relevantAt == null) {
      instance.relevantAt = Date.now();
    }
  });

  db.Measurement.addHook('afterCreate', async (instance) => {
    const competitorType = instance.dataValues.competitorType;
    const isJudgable = [
      MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE,
      MEASUREMENT_COMPETITOR_TYPE.UNRESOLVED,
    ].includes(competitorType);

    if (isJudgable) {
      const measurable = await instance.getMeasurable();
      await measurable.judged();
    }
  });
}

module.exports = {
  addHooks,
};
