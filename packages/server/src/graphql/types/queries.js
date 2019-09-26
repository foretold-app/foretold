const graphql = require('graphql');

const queries = new graphql.GraphQLEnumType({
  name: 'queries',
  values: {
    PERMISSIONS: { value: 'PERMISSIONS' },
    USER: { value: 'USER' },
    USERS: { value: 'USERS' },
    MEASUREMENT: { value: 'MEASUREMENT' },
    MEASUREMENTS: { value: 'MEASUREMENTS' },
    MEASURABLE: { value: 'MEASURABLE' },
    MEASURABLES: { value: 'MEASURABLES' },
    BOT: { value: 'BOT' },
    BOTS: { value: 'BOTS' },
    AGENT: { value: 'AGENT' },
    AGENTS: { value: 'AGENTS' },
    SERIES: { value: 'SERIES' },
    SERIES_COLLECTION: { value: 'SERIES_COLLECTION' },
    CHANNEL: { value: 'CHANNEL' },
    CHANNELS: { value: 'CHANNELS' },
    STATS: { value: 'STATS' },
  },
});

module.exports = {
  queries,
};
