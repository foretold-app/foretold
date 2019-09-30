const clientUrl = process.env.NODE_ENV === 'development'
  ? 'http://localhost:1234'
  : 'https://www.foretold.io';

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
 * @param measurable
 * @returns {string}
 */
const getMeasurableLink = (channel, measurable) => {
  return `${clientUrl}/c/${channel.id}/m/${measurable.id}`;
};

/**
 * @param channel
 * @returns {string}
 */
const getChannelLinkWithToken = (channel) => {
  return `${clientUrl}/c/${channel.id}#token={{token}}`;
};

/**
 * @param channel
 * @returns {string}
 */
const getChannelLink = (channel) => {
  return `${clientUrl}/c/${channel.id}`;
};

/**
 * @param user
 * @returns {string}
 */
const getAgentLinkWithToken = (user) => {
  return `${clientUrl}/agents/${user.agentId}#token={{token}}`;
};

/**
 * @param user
 * @returns {string}
 */
const getAgentLink = (user) => {
  return `${clientUrl}/agents/${user.agentId}`;
};

module.exports = {
  clientUrl,

  getMeasurableLinkWithToken,
  getMeasurableLink,

  getChannelLinkWithToken,
  getChannelLink,

  getAgentLinkWithToken,
  getAgentLink,
};
