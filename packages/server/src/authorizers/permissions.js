const _ = require('lodash');
const { shield, allow, and, or, not } = require('graphql-shield');

const { currentAgentIsAuthenticated } = require('./agents');
const { currentAgentIsApplicationAdmin } = require('./agents');
const { channelIsPublic } = require('./channels');
const { currentAgentIsChannelAdmin } = require('./channel-memberships');
const { currentAgentIsChannelViewer } = require('./channel-memberships');
const { channelHasMembershipWithCurrentAgent } = require('./channel-memberships');
const { channelHasMultipleAdmins } = require('./channel-memberships');
const { membershipBelongsToCurrentAgent } = require('./channel-memberships');
const { membershipHasAdminRole } = require('./channel-memberships');
const { measurableIsOwnedByCurrentAgent } = require('./measurables');
const { measurableIsArchived } = require('./measurables');
const { botBelongsToCurrentUser } = require('./bots');

const currentAgentIsApplicationAdminOrChannelAdmin = or(
  currentAgentIsApplicationAdmin,
  currentAgentIsChannelAdmin,
);

const rulesChannel = {
  Query: {},
  Mutation: {
    channelUpdate: and(
      currentAgentIsAuthenticated,
      currentAgentIsApplicationAdminOrChannelAdmin,
    ),
    leaveChannel: and(
      currentAgentIsAuthenticated,
      channelHasMembershipWithCurrentAgent,
      or(
        and(
          currentAgentIsApplicationAdminOrChannelAdmin,
          channelHasMultipleAdmins,
        ),
        not(currentAgentIsApplicationAdminOrChannelAdmin),
      ),
    ),
    joinChannel: and(
      currentAgentIsAuthenticated,
      channelIsPublic,
      not(channelHasMembershipWithCurrentAgent),
    ),
    channelMembershipCreate: and(
      currentAgentIsAuthenticated,
      currentAgentIsApplicationAdminOrChannelAdmin,
    ),
  }
};

const rulesChannelMemberships = {
  Query: {},
  Mutation: {
    channelMembershipDelete: and(
      currentAgentIsAuthenticated,
      currentAgentIsApplicationAdminOrChannelAdmin,
      or(
        and(membershipHasAdminRole, channelHasMultipleAdmins),
        not(membershipBelongsToCurrentAgent),
      ),
    ),
    channelMembershipRoleUpdate: and(
      currentAgentIsAuthenticated,
      currentAgentIsApplicationAdminOrChannelAdmin,
      or(
        and(channelHasMultipleAdmins, membershipBelongsToCurrentAgent),
        and(channelHasMultipleAdmins, membershipHasAdminRole),
        not(membershipBelongsToCurrentAgent),
      ),
    ),
  }
};

const rulesMeasurables = {
  Query: {},
  Mutation: {
    measurementCreate: and(
      currentAgentIsAuthenticated,
      or(
        channelIsPublic,
        or(
          currentAgentIsApplicationAdminOrChannelAdmin,
          currentAgentIsChannelViewer,
        ),
      ),
    ),
    measurableArchive: and(
      currentAgentIsAuthenticated,
      measurableIsOwnedByCurrentAgent,
      not(measurableIsArchived),
    ),
    measurableUnarchive: and(
      currentAgentIsAuthenticated,
      measurableIsOwnedByCurrentAgent,
      measurableIsArchived,
    ),
    measurableUpdate: and(
      currentAgentIsAuthenticated,
      measurableIsOwnedByCurrentAgent,
    ),
  }
};

const rules = {
  Bot: {
    token: botBelongsToCurrentUser,
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
    '*': currentAgentIsAuthenticated,
    botCreate: currentAgentIsAuthenticated,
    botUpdate: and(
      currentAgentIsAuthenticated,
      botBelongsToCurrentUser,
    ),
    channelCreate: currentAgentIsAuthenticated,
    userUpdate: currentAgentIsAuthenticated,
    seriesCreate: and(
      currentAgentIsAuthenticated,
      or(
        channelIsPublic,
        currentAgentIsApplicationAdminOrChannelAdmin,
      ),
    ),
    measurableCreate: and(
      currentAgentIsAuthenticated,
      or(
        channelIsPublic,
        or(
          currentAgentIsApplicationAdminOrChannelAdmin,
          currentAgentIsChannelViewer,
        ),
      ),
    ),

    ...rulesMeasurables.Mutation,
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
  rulesMeasurables,
  rulesChannelMemberships,
  getPermissions,
};
