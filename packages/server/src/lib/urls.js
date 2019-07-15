const clientUrl = process.env.NODE_ENV === "development"
  ? "http://localhost:1234"
  : "https://www.foretold.io";

const getMeasurableLink = (channel, measurable) => {
  return `${clientUrl}/c/${channel.id}/m/${measurable.id}`;
};

module.exports = {
  clientUrl,
  getMeasurableLink,
};
