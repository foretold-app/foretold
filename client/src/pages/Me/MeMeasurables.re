open Utils;
open Rationale;
open Table;
open Rationale.Function.Infix;
open HandsOnTable;
open MomentRe;

let matchId = [%re "/clickFn:id:(.*)/"];

let jsonToString = e => e |> Js.Json.decodeString |> Option.default("");
let optionalMoment = Option.Infix.(e => e <$> (jsonToString ||> moment));

module WithEditMutation = {
  module GraphQL = [%graphql
    {|
             mutation editMeasurable($id: String!, $name: String!, $description: String!, $expectedResolutionDate:Date) {
                 editMeasurable(id: $id, name: $name, description: $description, expectedResolutionDate: $expectedResolutionDate) {
                   id
                 }
             }
     |}
  ];

  module Mutation = ReasonApollo.CreateMutation(GraphQL);

  let mutate =
      (
        mutation: Mutation.apolloMutation,
        id: string,
        name: string,
        description: string,
        expectedResolutionDate: Js.Json.t,
      ) => {
    let m =
      GraphQL.make(~id, ~name, ~description, ~expectedResolutionDate, ());
    mutation(~variables=m##variables, ~refetchQueries=[|"getAgent"|], ())
    |> ignore;
  };
};

let notFound = <h3> {"Agent not found" |> ste} </h3>;

module WithAgent = {
  let query = (~id, innerFn) => {
    open Result.Infix;
    let query = Queries.GetUserMeasurables.Query.make(~id, ());
    Queries.GetUserMeasurables.QueryComponent.make(
      ~variables=query##variables, ({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      <$> (e => e##agent)
      >>= (
        e =>
          switch (e) {
          | Some(a) => Ok(a)
          | None => Error(notFound)
          }
      )
      <$> innerFn
      |> Result.result(idd, idd)
    )
    |> ReasonReact.element;
  };
};

let component = ReasonReact.statelessComponent("MeMeasurables");

let make = (~id: string, _children) => {
  ...component,
  render: _ =>
    WithAgent.query(
      ~id,
      agent => {
        let m =
          agent##measurables
          |> ArrayOptional.concatSomes
          |> Array.map(Queries.GetUserMeasurables.toMeasurable);
        EditMe.make(~measurables=m, ());
      },
    )
    |> FillWithSidebar.make(~channel=None)
    |> ReasonReact.element,
};