const { channel } = require('./channels');
const { channelMemberships } = require('./channel-memberships');
const { measurable } = require('./measurables');

/**
 * Do not try to use DRY principle here.
 * Just read each section as it is.
 */

const middlewares = {
  Mutation: {
    createSeries: async (resolve, root, args, context, info) => {
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    createMeasurement: async (resolve, root, args, context, info) => {
      await measurable(root, args, context, info);
      await channel(root, args, context, info);
      await channelMemberships(root, args, context, info);
      return await resolve(root, args, context, info);
    },

    createMeasurable: async (resolve, root, args, context, info) => {
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
  }
};

module.exports = {
  middlewares,
};
