const _ = require('lodash');

const { setContextChannel, setContextChannelByRoot } = require('./channels');
const { setContextChannelMemberships } = require('./channel-memberships');
const { setContextChannelMembershipsAdmins } = require('./channel-memberships');
const { setContextMeasurable } = require('./measurables');
const { setContextMeasurableByRoot } = require('./measurables');
const { measurementValueValidation } = require('./measurements');
const { measurableStateValidation } = require('./measurements');
const { formatResponseIntoConnection } = require('./connections');
const { setContextBot } = require('./bots');

/**
 * Do not try to use DRY principle here.
 * Just read each section as it is.
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
  },

  Measurable: {
    permissions: async (resolve, root, args, context, info) => {
      await setContextMeasurableByRoot(root, args, context, info);
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      return resolve(root, args, context, info);
    },
  },

  Channel: {
    permissions: async (resolve, root, args, context, info) => {
      context = _.cloneDeep(context);
      await setContextChannelByRoot(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      await setContextChannelMembershipsAdmins(root, args, context, info);
      return resolve(root, args, context, info);
    },
  },

  ChannelsMembership: {
    permissions: async (resolve, root, args, context, info) => {
      context = _.cloneDeep(context);
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      await setContextChannelMembershipsAdmins(root, args, context, info);
      return resolve(root, args, context, info);
    },
  },

  Query: {
    permissions: async (resolve, root, args, context, info) => {
      await setContextMeasurable(root, args, context, info);
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      await setContextChannelMembershipsAdmins(root, args, context, info);
      return resolve(root, args, context, info);
    },

    measurements: async (resolve, root, args, context, info) => {
      const result = await resolve(root, args, context, info);
      return formatResponseIntoConnection(result, root, args, context, info);
    },

    measurables: async (resolve, root, args, context, info) => {
      const result = await resolve(root, args, context, info);
      return formatResponseIntoConnection(result, root, args, context, info);
    },

    bots: async (resolve, root, args, context, info) => {
      const result = await resolve(root, args, context, info);
      return formatResponseIntoConnection(result, root, args, context, info);
    },
  },

  Mutation: {
    seriesCreate: async (resolve, root, args, context, info) => {
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      return resolve(root, args, context, info);
    },

    measurementCreate: async (resolve, root, args, context, info) => {
      await measurementValueValidation(root, args, context, info);
      await setContextMeasurable(root, args, context, info);
      await measurableStateValidation(root, args, context, info);
      await setContextChannel(root, args, context, info);
      await setContextChannelMemberships(root, args, context, info);
      return resolve(root, args, context, info);
    },

    measurableCreate: async (resolve, root, args, context, info) => {
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
  }
};

module.exports = {
  middlewares,
};
