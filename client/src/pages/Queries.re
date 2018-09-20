open Rationale;
open Rationale.Option;

module GetUser = [%graphql
  {|
    query getUser ($id: String!) {
        user:
          user(id: $id){
            id
            name
        }
    }
  |}
];
module GetUserQuery = ReasonApollo.CreateQuery(GetUser);

module GetUsers = [%graphql
  {|
    query getUsers {
        users {
           id
           name
           createdAt
           updatedAt
        }
    }
  |}
];

module GetUsersQuery = ReasonApollo.CreateQuery(GetUsers);

let stringOfcompetitorType = e =>
  switch (e) {
  | `AGGREGATION => "Aggregation"
  | `COMPETITIVE => "Competitive"
  | `OBJECTIVE => "Objective"
  };

type user = {
  id: string,
  name: string,
};

type bot = {
  competitorType: [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE],
  description: option(string),
  id: string,
  name: option(string),
};

type agent = {
  id: string,
  bot: option(bot),
  user: option(user),
};

type agents = array(agent);

module GetAgents = [%graphql
  {|
    query getAgents {
      agents @bsRecord {
        id
        user: User @bsRecord{
          id
          name
        }
        bot: Bot @bsRecord{
          id
          name
          description
          competitorType
        }
      }
    }
  |}
];

module GetAgentsQuery = ReasonApollo.CreateQuery(GetAgents);

type measurable = {
  id: string,
  name: string,
  createdAt: string,
  updatedAt: string,
};

type measurables = array(measurable);

let jsonToString = e => e |> Js.Json.decodeString |> Option.default("");

module GetMeasurables = [%graphql
  {|
    query getMeasurables {
        measurables @bsRecord {
           id
           name
           createdAt @bsDecoder(fn: "jsonToString")
           updatedAt @bsDecoder(fn: "jsonToString")
        }
    }
  |}
];
module GetMeasurablesQuery = ReasonApollo.CreateQuery(GetMeasurables);