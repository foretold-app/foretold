const graphql = require("graphql");

const mutations = new graphql.GraphQLEnumType({
  name: 'mutations',
  values: {
    measurementCreate: { value: "measurementCreate" },
    measurableCreate: { value: "measurableCreate" },
    seriesCreate: { value: "seriesCreate" },
    measurableArchive: { value: "measurableArchive" },
    measurableUnarchive: { value: "measurableUnarchive" },
    measurableUpdate: { value: "measurableUpdate" },
    userUpdate: { value: "userUpdate" },
    channelUpdate: { value: "channelUpdate" },
    channelCreate: { value: "channelCreate" },
    channelMembershipCreate: { value: "channelMembershipCreate" },
    channelMembershipRoleUpdate: { value: "channelMembershipRoleUpdate" },
    channelMembershipDelete: { value: "channelMembershipDelete" },
    leaveChannel: { value: "leaveChannel" },
    joinChannel: { value: "joinChannel" },
  }
});

module.exports = {
  mutations,
};
