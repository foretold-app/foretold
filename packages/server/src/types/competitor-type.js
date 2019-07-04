const graphql = require("graphql");

const competitorType = graphql.GraphQLNonNull(new graphql.GraphQLEnumType({
  name: 'competitorType',
  values: {
    COMPETITIVE: { value: "COMPETITIVE" },
    AGGREGATION: { value: "AGGREGATION" },
    OBJECTIVE: { value: "OBJECTIVE" },
  }
}));

module.exports = {
  competitorType,
};
