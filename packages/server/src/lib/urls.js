const clientUrl = process.env.NODE_ENV === "development"
  ? "http://localhost:1234"
  : "https://www.foretold.io";

/**
 * @param channel
 * @param measurable
 * @returns {string}
 */
const getMeasurableLinkWithToken = (channel, measurable) => {
  return `${clientUrl}/c/${channel.id}/m/${measurable.id}#token={{token}}`;
};

/**
 * @param channel
 * @returns {string}
 */
const getChannelLinkWithToken = (channel) => {
  return `${clientUrl}/c/${channel.id}#token={{token}}`;
};

/**
 * @param user
 * @returns {string}
 */
const getAgentLinkWithToken = (user) => {
  return `${clientUrl}/agents/${user.agentId}#token={{token}}`;
};

module.exports = {
  clientUrl,
  getMeasurableLinkWithToken,
  getChannelLinkWithToken,
  getAgentLinkWithToken,
};
