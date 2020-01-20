const _ = require('lodash');

const { setContextChannel, setContextChannelByRoot } = require('./channels');
const { setContextChannelMemberships } = require('./channel-memberships');
const { setContextChannelMembershipsAdmins } = require('./channel-memberships');
const { setContextMeasurable } = require('./measurables');
const { setContextMeasurableByRoot } = require('./measurables');
const { measurableNameValidation } = require('./measurables');
const { measurementValueValidation } = require('./measurements');
const { measurementValueTypeValidation } = require('./measurements');
const { setContextBot } = require('./bots');
const { setContextPreferenceFromId } = require('./preferences');
const { setContextPreferenceFromAgentId } = require('./preferences');
const { setContextUser } = require('./users');
const { setContextNotebook } = require('./notebooks');
const { authenticationInputValidation } = require('./authentications');
const { competitiveMeasurementCanBeAddedToOpenMeasurable }
  = require('./measurements');

/**
 * Do not try to use DRY principle here.
 * Just read each section as it is.
 *
 * Be super-careful! Remember that a bunch of middlewares is run synchronously.
 * But each "middleware" works asynchronously.
 */
const middlewares = {
  Bot: {
    token: async (resolve, root, args, context, info) => {
      const result = await resolve(root, args, context, info);
      return (result instanceof Error) ? null : result;
    },
  },

  User: {
    email: async (resolve, root, args, context, info) => {
      const result = await resolve(root, args, context, info);
      return (result instanceof Error) ? null : result;
    },
    auth0Id: async (resolve, root, args, context, info) => {
      const result = await resolve(root, args, context, info);
      return (result instanceof Error) ? null : result;
    },
    isEmailVerified: async (resolve, root, args, context, info) => {
      const result = await resolve(root, args, context, info);
      return (result instanceof Error) ? null : result;
    },
  },

  Agent: {
    preference: async (resolve, root, args, context, info) => {
      const result = await resolve(root, args, context, info);
      return (result instanceof Error) ? null : result;
    },
  },

  // Copy context here (temporary solution)
  Measurable: {
    permissions: async (resolve, root, args, context, info) => {
      const context$ = _.cloneDeep(context);
      await setContextMeasurableByRoot(root, args, context$, info);
      await setContextChannel(root, args, context$, info);
      await setContextChannelMemberships(root, args, context$, info);
      return resolve(root, args, context$, info);
    },
  },

  // Copy context here (temporary solution)
  Channel: {
    permissions: async (resolve, root, args, context, info) => {
      const context$ = _.cloneDeep(context);
      await setContextChannelByRoot(root, args, context$, info);
      await setContextChannelMemberships(root, args, context$, info);
      await setContextChannelMembershipsAdmins(root, args, context$, info);
      return resolve(root, args, context$, info);
    },
  },

  // Copy context here (temporary solution)
  ChannelsMembership: {
    permissions: async (resolve, root, args, context, info) => {
      const context$ = _.cloneDeep(context);
      await setContextChannel(root, args, context$, info);
      await setContextChannelMemberships(root, args, context$, info);
      await setContextChannelMembershipsAdmins(root, args, context$, info);
      return resolve(root, args, context$, info);
    },
  },

  Query: {
    authentication: async (resolve, root, args, context, info) => {
      await authenticationInputValidation(root, args, context, info);
      return resolve(root, args, context, info);
    },

    permissions: async (resolve, root, args, context, info) => {
      await setContextMeasurable(root, args, context, info);
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      await setContextChannelMembershipsAdmins(root, args, context, info);
      return resolve(root, args, context, info);
    },
  },

  Mutation: {
    notebookDelete: async (resolve, root, args, context, info) => {
      await setContextNotebook(root, args, context, info);
      return resolve(root, args, context, info);
    },

    seriesCreate: async (resolve, root, args, context, info) => {
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      return resolve(root, args, context, info);
    },

    measurementCreate: async (resolve, root, args, context, info) => {
      await setContextMeasurable(root, args, context, info);
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);

      await measurementValueValidation(root, args, context, info);
      await measurementValueTypeValidation(root, args, context, info);
      await competitiveMeasurementCanBeAddedToOpenMeasurable(
        root, args, context, info,
      );

      return resolve(root, args, context, info);
    },

    measurableCreate: async (resolve, root, args, context, info) => {
      await measurableNameValidation(root, args, context, info);
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      return resolve(root, args, context, info);
    },

    channelUpdate: async (resolve, root, args, context, info) => {
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      return resolve(root, args, context, info);
    },

    channelMembershipCreate: async (resolve, root, args, context, info) => {
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      await setContextChannelMembershipsAdmins(root, args, context, info);
      return resolve(root, args, context, info);
    },

    channelMembershipRoleUpdate: async (resolve, root, args, context, info) => {
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      await setContextChannelMembershipsAdmins(root, args, context, info);
      return resolve(root, args, context, info);
    },

    channelMembershipDelete: async (resolve, root, args, context, info) => {
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      await setContextChannelMembershipsAdmins(root, args, context, info);
      return resolve(root, args, context, info);
    },

    invitationCreate: async (resolve, root, args, context, info) => {
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      await setContextChannelMembershipsAdmins(root, args, context, info);
      return resolve(root, args, context, info);
    },

    measurableArchive: async (resolve, root, args, context, info) => {
      await setContextMeasurable(root, args, context, info);
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      return resolve(root, args, context, info);
    },

    measurableUnarchive: async (resolve, root, args, context, info) => {
      await setContextMeasurable(root, args, context, info);
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      return resolve(root, args, context, info);
    },

    measurableUpdate: async (resolve, root, args, context, info) => {
      await setContextMeasurable(root, args, context, info);
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      return resolve(root, args, context, info);
    },

    leaveChannel: async (resolve, root, args, context, info) => {
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      await setContextChannelMembershipsAdmins(root, args, context, info);
      return resolve(root, args, context, info);
    },

    joinChannel: async (resolve, root, args, context, info) => {
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      await setContextChannelMembershipsAdmins(root, args, context, info);
      return resolve(root, args, context, info);
    },

    botUpdate: async (resolve, root, args, context, info) => {
      await setContextBot(root, args, context, info);
      return resolve(root, args, context, info);
    },

    preferenceUpdate: async (resolve, root, args, context, info) => {
      await setContextPreferenceFromId(root, args, context, info);
      return resolve(root, args, context, info);
    },

    subscribe: async (resolve, root, args, context, info) => {
      await setContextPreferenceFromAgentId(root, args, context, info);
      return resolve(root, args, context, info);
    },

    unsubscribe: async (resolve, root, args, context, info) => {
      await setContextPreferenceFromAgentId(root, args, context, info);
      return resolve(root, args, context, info);
    },

    userUpdate: async (resolve, root, args, context, info) => {
      await setContextUser(root, args, context, info);
      return resolve(root, args, context, info);
    },
  },
};

module.exports = {
  middlewares,
};
