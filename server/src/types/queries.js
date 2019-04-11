const graphql = require("graphql");

const queries = new graphql.GraphQLEnumType({
  name: 'queries',
  values: {
    permissions: { value: "permissions" },
    user: { value: "user" },
    users: { value: "users" },
    measurement: { value: "measurement" },
    measurements: { value: "measurements" },
    measurable: { value: "measurable" },
    measurables: { value: "measurables" },
    bot: { value: "bot" },
    bots: { value: "bots" },
    agent: { value: "agent" },
    agents: { value: "agents" },
    series: { value: "series" },
    seriesCollection: { value: "seriesCollection" },
    channel: { value: "channel" },
    channels: { value: "channels" },
    stats: { value: "stats" },
  }
});

module.exports = {
  queries,
};
