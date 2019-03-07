const graphql = require("graphql");

let valueType = graphql.GraphQLNonNull(new graphql.GraphQLEnumType({
  name: 'valueType',
  values: {
    FLOAT: { value: "FLOAT" },
    DATE: { value: "DATE" },
    PERCENTAGE: { value: "PERCENTAGE" }
  }
}));

module.exports = {
  valueType,
};
