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
query search ($measurableId: String) {
  measurements(
    first: 500
    measurableId: $measurableId
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
mutation measurementCreate($input: MeasurementCreateInput!) {
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
