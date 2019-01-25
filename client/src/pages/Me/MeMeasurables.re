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
      |> apolloResponseToResult
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

let logThis = (ref, _) => Js.log(ref);

type resultData = array(array(string));

type item = {
  name: string,
  description: string,
  isLocked: bool,
  expectedResolutionDate: string,
};

let toItem = (r: array(string)): item => {
  name: r[0],
  description: r[1],
  isLocked: r[2] == "true" ? true : false,
  expectedResolutionDate: r[3],
};

type reactRef = ref(option(ReasonReact.reactRef));
type state = {hotTableRef: reactRef};

type action =
  | Save;

let component = ReasonReact.reducerComponent("MeMeasurables");

let setSectionRef = (theRef, {ReasonReact.state}) =>
  state.hotTableRef := Js.Nullable.toOption(theRef);

let toItems = (hotTableRef: reactRef): array(item) =>
  switch (hotTableRef^) {
  | None => [||]
  | Some(r) =>
    let data: resultData = ReasonReact.refToJsObj(r)##hotInstance##getData();
    Js.log2("HIHI", data);
    Array.map(toItem, data);
  };

let make = (~id: string, _children) => {
  ...component,
  reducer: (action: action, state: state) =>
    switch (action) {
    | Save =>
      Js.log(toItems(state.hotTableRef));
      ReasonReact.NoUpdate;
    },
  initialState: () => {hotTableRef: ref(None)},
  shouldUpdate: a => false,
  render: self =>
    WithEditMutation.Mutation.make((mutation, _) =>
      WithAgent.query(
        ~id,
        agent => {
          let m =
            agent##measurables
            |> ArrayOptional.concatSomes
            |> Array.map(Queries.GetUserMeasurables.toMeasurable);
          <StopComponentUpdate>
            {EditMe.make(~measurables=m, ())}
          </StopComponentUpdate>;
        },
      )
    )
    |> ReasonReact.element,
};