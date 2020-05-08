const authenticated = `
query {
  authenticated {
    agent { id }
  }
}
`;

const measurables = `
query {
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
query measurements(
  $measurableId: String
  $competitorType: [competitorType!]
  $notTaggedByAgent: String
  $findInDateRange: MeasurementsInDateRangeInput
) {
  measurements(
    first: 500
    measurableId: $measurableId
    competitorType: $competitorType
    notTaggedByAgent: $notTaggedByAgent
    findInDateRange: $findInDateRange
    isVerified: true
    isCancelled: false
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

const mutexTake = `
mutation mutexTake(
  $name: String
) {
  mutexTake(input:{
    name: $name
  }){
    id
  }
}
`;

const mutexFree = `
mutation mutexFree(
  $id: String!
) {
  mutexFree(id: $id)
}
`;

module.exports = {
  authenticated,
  measurables,
  measurements,
  measurementCreate,
  mutexTake,
  mutexFree,
};
