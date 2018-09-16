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