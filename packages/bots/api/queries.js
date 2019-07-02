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
      }
    }
  }
}
`;

const measurements = `
query search (
  $measurableId: String
  $competitorType: [competitorType!]
  $notTaggedByAgent: String
  $findInDateRange: MeasurementsInDateRangeInput
) {
  measurements(
    first: 200
    measurableId: $measurableId
    competitorType: $competitorType
    notTaggedByAgent: $notTaggedByAgent
    findInDateRange: $findInDateRange
  ) {
    edges {
      node {
        id
        value {
          floatCdf { xs ys } 
          floatPoint 
          percentage 
          binary
        }
        measurableId
        createdAt
        agentId
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
    __typename
  }
}
`;

module.exports = {
  measurables,
  measurements,
  measurementCreate,
};
