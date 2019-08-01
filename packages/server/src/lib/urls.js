const clientUrl = process.env.NODE_ENV === "development"
  ? "http://localhost:1234"
  : "https://www.foretold.io";


/**
 * @param channel
 * @param measurable
 * @returns {string}
 */
const getMeasurableLink = (channel, measurable) => {
  return `${clientUrl}/c/${channel.id}/m/${measurable.id}`;
};

/**
 * @param channel
 * @param measurable
 * @returns {string}
 */
const getMeasurableLinkWithToken = (channel, measurable) => {
  return `${getMeasurableLink(channel, measurable)}#token={{token}}`;
};

/**
 * @param channel
 * @returns {string}
 */
const getChannelLink = (channel) => {
  return `${clientUrl}/c/${channel.id}`;
};

/**
 * @param channel
 * @returns {string}
 */
const getChannelLinkWithToken = (channel) => {
  return `${getChannelLink(channel)}#token={{token}}`;
};

/**
 * @param user
 * @returns {string}
 */
const getAgentLink = (user) => {
  return `${clientUrl}/agents/${user.agentId}`;
};


/**
 * @param user
 * @returns {string}
 */
const getAgentLinkWithToken = (user) => {
  return `${getAgentLink(user)}#token={{token}}`;
};


/**
 * @returns {string}
 */
const getInvitationLink = () => {
  return `${clientUrl}/invitation`;
};

/**
 * @param {Models.Invitation} invitation
 * @returns {string}
 */
const getInvitationLinkWithInvitationToken = (invitation) => {
  return `${clientUrl}/invitation#invitationToken=${invitation.token}`;
};

module.exports = {
  clientUrl,
  getMeasurableLink,
  getMeasurableLinkWithToken,

  getChannelLink,
  getChannelLinkWithToken,

  getAgentLink,
  getAgentLinkWithToken,

  getInvitationLink,
  getInvitationLinkWithInvitationToken,
};
