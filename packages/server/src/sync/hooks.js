const events = require('./events');
const emitter = require('./emitter');
const logger = require('../lib/log');

const log = logger.module('sync/hooks');

/**
 * Try to keep all "sync" hooks in one place.
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
 * So this is why we keep "sync" part of the application here, and
 * "associations" definitions too. It does not matter where the "associations"
 * are defined and placed (either DB or JS files). The main purpose
 * is to give to developers to read this base definitions without
 * duplications and keep in a mind only a small part of this.
 */

function addHooks(db) {
  /** afterUpdate */
  db.Measurable.addHook('afterUpdate', (instance) => {
    try {
      emitter.emit(events.MEASURABLE_CHANGED, instance);
    } catch (e) {
      log.trace('Hook MEASURABLE_CHANGED', e);
    }
  });
  db.User.addHook('afterUpdate', (instance) => {
    try {
      emitter.emit(events.USER_CHANGED, instance);
    } catch (e) {
      log.trace('Hook USER_CHANGED', e);
    }
  });
  db.Series.addHook('afterUpdate', (instance) => {
    try {
      emitter.emit(events.SERIES_CHANGED, instance);
    } catch (e) {
      log.trace('Hook SERIES_CHANGED', e);
    }
  });

  /** afterCreate */
  db.Measurement.addHook('afterCreate', (instance) => {
    try {
      emitter.emit(events.NEW_MEASUREMENT, instance);
    } catch (e) {
      log.trace('Hook NEW_MEASUREMENT', e);
    }
  });
  db.Measurable.addHook('afterCreate', (instance) => {
    try {
      emitter.emit(events.NEW_MEASURABLE, instance);
    } catch (e) {
      log.trace('Hook NEW_MEASURABLE', e);
    }
  });
  db.ChannelMemberships.addHook('afterCreate', (instance) => {
    try {
      emitter.emit(events.NEW_MEMBERSHIP, instance);
    } catch (e) {
      log.trace('Hook NEW_MEMBERSHIP', e);
    }
  });
  db.Channel.addHook('afterCreate', (instance) => {
    try {
      emitter.emit(events.NEW_CHANNEL, instance);
    } catch (e) {
      log.trace('Hook NEW_CHANNEL', e);
    }
  });
  db.Series.addHook('afterCreate', (instance) => {
    try {
      emitter.emit(events.NEW_SERIES, instance);
    } catch (e) {
      log.trace('Hook NEW_SERIES', e);
    }
  });
  db.Invitation.addHook('afterCreate', (instance) => {
    try {
      emitter.emit(events.NEW_INVITATION, instance);
    } catch (e) {
      log.trace('Hook NEW_INVITATION', e);
    }
  });
  db.User.addHook('afterCreate', (instance) => {
    try {
      emitter.emit(events.NEW_USER, instance);
    } catch (e) {
      log.trace('Hook NEW_USER', e);
    }
  });
  db.Notebook.addHook('afterCreate', (instance) => {
    try {
      emitter.emit(events.NEW_NOTEBOOK, instance);
    } catch (e) {
      log.trace('Hook NEW_NOTEBOOK', e);
    }
  });

  /** beforeCreate */
  db.Bot.addHook('beforeCreate', async (instance) => {
    try {
      await emitter.emitAsync(events.CREATING_BOT, instance);
    } catch (e) {
      log.trace('Hook CREATING_BOT', e);
    }
  });
  db.User.addHook('beforeCreate', async (instance) => {
    try {
      await emitter.emitAsync(events.CREATING_USER, instance);
    } catch (e) {
      log.trace('Hook CREATING_USER', e);
    }
  });

  /** beforeUpdate */
  db.Measurable.addHook('beforeUpdate', async (instance) => {
    try {
      await emitter.emitAsync(events.UPDATING_MEASURABLE, instance);
    } catch (e) {
      log.trace('Hook Measurable beforeUpdate', e);
    }
  });

  /** beforeValidate */
  db.Measurement.addHook('beforeValidate', async (instance) => {
    try {
      await emitter.emitAsync(events.VALIDATING_MEASUREMENT, instance);
    } catch (e) {
      log.trace('Hook Measurement beforeValidate', e);
    }
  });
}

module.exports = {
  addHooks,
};
