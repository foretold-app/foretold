const events = require('../async/events');
const emitter = require('../async/emitter');

const { AGENT_TYPE } = require('./enums/agent-type');
const { MEASUREMENT_COMPETITOR_TYPE } = require('./enums/measurement-competitor-type');

/**
 * Try to keep all "async" hooks in one place.
 * For what do you want to ask?
 *
 * Server side application architecture is being building with separated,
 * decomposed layers (Model Definitions, Models Abstractions, Data Layer.
 * Middleware Layer, Authorizers Layer, Async Flows Layer, GrqphQL
 * Layer ans its resolvers).
 *
 * Models are just "data type definitions" which are similar with "grapqhl
 * types". "Sequelize" is a just lib which plays role of "DB Driver".
 *
 * So this is why we keep "async" part of the application here, and
 * "associations" definitions too. It does not matter where the "associations"
 * are defined and placed (either DB or JS files). The main purpose
 * is to give to developers to read this base definitions without
 * duplications and keep in a mind only a small part of this.
 *
 */

function addHooks(db) {
  db.sequelize.addHook('afterUpdate', (instance) => {
    if (
      instance instanceof db.Measurable &&
      instance.changed('state')
    ) {
      emitter.emit(events.MEASURABLE_STATE_IS_CHANGED, instance);
    }
  });

  db.sequelize.addHook('afterUpdate', (instance) => {
    if (
      instance instanceof db.Measurable &&
      instance.changed('state')
    ) {
      emitter.emit(events.MEASURABLE_STATE_IS_CHANGED, instance);
    }
  });

  db.sequelize.addHook('afterCreate', (instance) => {
    if (instance instanceof db.ChannelMemberships) {
      emitter.emit(events.MEMBER_ADDED_TO_COMMUNITY, instance);
    }
  });

  // db.sequelize.addHook('afterCreate', (instance) => {
  //   if (instance instanceof db.Invitation) {
  //     emitter.emit(events.MEMBER_INVITED_TO_COMMUNITY, instance);
  //   }
  // });

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
