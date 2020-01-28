const authenticated = `
query {
  authenticated {
    agent { id }
  }
}
`;

const authentication = `
query authentication(
  $auth0jwt: JWT!
  $auth0accessToken: String
){
  authentication(
    auth0jwt: $auth0jwt
    auth0accessToken: $auth0accessToken
  ){
    jwt
  }
}
`;

const measurables = `
query measurables(
  $first: Int500
  $states: [measurableState]
  $isArchived: [isArchived]
  $channelId: String
) {
  measurables(
    first: $first
    states: $states
    isArchived: $isArchived 
    channelId: $channelId
  ) {
    edges {
      node {
        id
        name
        valueType
        state
        channelId
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
    id
    createdAt
  }
}
`;

const channelCreate = `
mutation channelCreate($input: ChannelInput!) {
  channelCreate(
    input: $input
  ) {
    id
    name
    notebooksCount
    openedMeasurablesCount
    isPublic
    isCurated
    isArchived
    permissions {
      queries {
        deny
        allow
      }
      mutations {
        deny
        allow
      }
    }
    description
    membershipCount
  }
}
`;

const measurableCreate = `
mutation measurableCreate($input: MeasurableCreateInput!) {
  measurableCreate(input: $input) {
    id
    channelId
    createdAt
    updatedAt
    valueType
    expectedResolutionDate
    resolutionEndpointResponse
    labelCustom
    permissions {
      queries {
        allow
        deny
      }
      mutations {
        allow
        deny
      }
    }
    resolutionEndpoint
    labelSubject
    labelProperty
    labelOnDate
    labelCustom
  }
}
`;

const channel = `
query channel($id: String!) {
  channel(id: $id) {
    id
    name
    description
    isArchived
    isPublic
    membershipCount
    notebooksCount
    myRole
    permissions {
      mutations {
        allow
      }
    }
  }
}
`;

module.exports = {
  authenticated,
  measurables,
  measurements,
  measurementCreate,
  authentication,
  channelCreate,
  measurableCreate,
  channel,
};
