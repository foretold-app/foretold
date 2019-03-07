const graphql = require("graphql");

let competitor = graphql.GraphQLNonNull(new graphql.GraphQLEnumType({
  name: 'competitorType',
  values: {
    COMPETITIVE: { value: "COMPETITIVE" },
    AGGREGATION: { value: "AGGREGATION" },
    OBJECTIVE: { value: "OBJECTIVE" }
  }
}));

modules.exports = {
  competitor,
};
