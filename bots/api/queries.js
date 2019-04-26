const measurables = `
query measurables {
  measurables(
    first: 500
    states: [OPEN, JUDGEMENT_PENDING] 
    isArchived: [FALSE]
  ) {
      edges {
        node {
          id
          name
          valueType
          state
          aggregatedAt
      }
    }
  }
}
`;

const measurements = `
query search (
  $measurableId: String
  $competitorType: [competitorType!]
  $createdAfter: Date
) {
  measurements(
    first: 500
    measurableId: $measurableId
    competitorType: $competitorType
    createdAfter: $createdAfter
  ) {
    edges {
      node {
        id
        value { floatCdf { xs ys } floatPoint }
      }
    }
  }
}
`;

const measurementCreate = `
mutation measurementCreate(
  $input: MeasurementCreateInput!
) {
  measurementCreate(input: $input) {
    createdAt
  }
}
`;

const measurableAggregate = `
mutation measurableAggregate(
  $measurableId: String!
) {
  measurableAggregate(
    id: $measurableId
  ) {
    aggregatedAt
  }
}
`;

module.exports = {
  measurables,
  measurements,
  measurementCreate,
  measurableAggregate,
};
