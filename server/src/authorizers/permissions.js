const _ = require('lodash');
const { shield, allow, and, or, not } = require('graphql-shield');

const { isAuthenticated } = require('./agents');
const { isAdmin, isViewer, isInChannel } = require('./channel-memberships');
const { isChannelPublic } = require('./channels');
const measurables = require('./measurables');
const bots = require('./bots');

const rulesChannel = {
  Query: {},
  Mutation: {
    channelUpdate: and(isAuthenticated, isAdmin),
    leaveChannel: and(isAuthenticated, isInChannel),
    joinChannel: and(isAuthenticated, isChannelPublic, not(isInChannel)),
    channelMembershipCreate: and(isAuthenticated, isAdmin),
  }
};

const rulesChannelMemberships = {
  Query: {},
  Mutation: {
    channelMembershipDelete: and(isAuthenticated, isAdmin),
    channelMembershipRoleUpdate: and(isAuthenticated, isAdmin),
  }
};

const rules = {
  Bot: {
    jwt: bots.isOwner,
  },
  Query: {
    '*': allow,
    permissions: allow,
    user: allow,
    users: allow,
    measurement: allow,
    measurements: allow,
    measurable: allow,
    measurables: allow,
    bot: allow,
    bots: allow,
    agent: allow,
    agents: allow,
    series: allow,
    seriesCollection: allow,
    channel: allow,
    channels: allow,
    stats: allow,
  },
  Mutation: {
    '*': isAuthenticated,
    botCreate: isAuthenticated,
    channelCreate: isAuthenticated,
    userUpdate: isAuthenticated,
    measurementCreate: and(isAuthenticated, or(isChannelPublic, or(isAdmin, isViewer))),
    seriesCreate: and(isAuthenticated, or(isChannelPublic, isAdmin)),

    measurableCreate: and(isAuthenticated, or(isChannelPublic, or(isAdmin, isViewer))),
    measurableArchive: and(isAuthenticated, measurables.isOwner),
    measurableUnarchive: and(isAuthenticated, measurables.isOwner),
    measurableUpdate: and(isAuthenticated, measurables.isOwner),

    ...rulesChannel.Mutation,
    ...rulesChannelMemberships.Mutation,
  }
};

function getPermissions() {
  return shield(
    _.cloneDeep(rules),
    { debug: false }
  );
}

module.exports = {
  rules,
  rulesChannel,
  rulesChannelMemberships,
  getPermissions,
};
