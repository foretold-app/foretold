const clientUrl = process.env.NODE_ENV === "development"
  ? "http://localhost:1234"
  : "https://www.foretold.io";

const getMeasurableLinkWithToken = (channel, measurable) => {
  return `${clientUrl}/c/${channel.id}/m/${measurable.id}#token={{token}}`;
};

module.exports = {
  clientUrl,
  getMeasurableLinkWithToken,
};
