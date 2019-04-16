const _ = require('lodash');

const { channel, channelByRoot } = require('./channels');
const { channelMemberships } = require('./channel-memberships');
const { measurable } = require('./measurables');

/**
 * Do not try to use DRY principle here.
 * Just read each section as it is.
 */

const middlewares = {
  Bot: {
    jwt: async (resolve, root, args, context, info) => {
      const result = await resolve(root, args, context, info);
      return (result instanceof Error) ? null : result;
    },
  },

  Channel: {
    permissions: async (resolve, root, args, context, info) => {
      context = _.cloneDeep(context);
      await channelByRoot(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },
  },

  ChannelsMembership: {
    permissions: async (resolve, root, args, context, info) => {
      context = _.cloneDeep(context);
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },
  },

  Query: {
    permissions: async (resolve, root, args, context, info) => {
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },
  },

  Mutation: {
    seriesCreate: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    measurementCreate: async (resolve, root, args, context, info) => {
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    measurableCreate: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    channelUpdate: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    channelMembershipCreate: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    channelMembershipRoleUpdate: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    channelMembershipDelete: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    measurableArchive: async (resolve, root, args, context, info) => {
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    measurableUnarchive: async (resolve, root, args, context, info) => {
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    measurableUpdate: async (resolve, root, args, context, info) => {
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    leaveChannel: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    joinChannel: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },
  }
};

module.exports = {
  middlewares,
};
