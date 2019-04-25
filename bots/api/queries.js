const measurables = `
query measurables {
  measurables(first: 500, states: [OPEN, JUDGEMENT_PENDING], isArchived: [FALSE]) {
      total
      edges {
        node {
          id
          name
          valueType
          state
          Measurements(first:1){
            edges {
               node {
                 id
                 value {
                    floatCdf { xs ys }
                    floatPoint
                 }
               }
            }
          }
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

input MeasurementCreateInput {
  measurableId: String
  author: String
  value: MeasurementValueInput
  competitorType: competitorType!
}

input MeasurementValueInput {
  floatCdf: MeasurementValueInputFloatCdf
  floatPoint: Float
}

enum competitorType {
  COMPETITIVE
  AGGREGATION
  OBJECTIVE
}

type MeasurementValueInputFloatCdf {
  xs: [Float]!
  ys: [Float]!
}
`;

module.exports = {
  measurables,
  measurementCreate,
};
