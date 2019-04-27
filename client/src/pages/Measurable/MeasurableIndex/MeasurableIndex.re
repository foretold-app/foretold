open Utils;
open Foretold__GraphQL;
open Rationale.Function.Infix;

module Types = Measurable__Index__Logic;
module Components = Measurable__Index__Components;

module KeyValuePairs = {
  type t = {
    key: string,
    value: string,
  };

  type ts = array(t);

  let fromSearchParam = (r: string) =>
    r
    |> Js.String.split("&")
    |> E.A.fmap(r => {
         let param = Js.String.split("=", r) |> E.A.to_list;
         switch (param) {
         | [key, value] => Some({key, value})
         | _ => None
         };
       })
    |> E.A.O.concatSome;

  let get = (key, ts: ts) =>
    ts
    |> E.A.filter(r => r.key == key)
    |> E.A.get(_, 0)
    |> E.O.fmap(r => r.value);
};

module SearchResults = {
  type query = {state: option(Context.Primary.MeasurableState.t)};

  let stateFromString =
      (r: string): option(Context.Primary.MeasurableState.t) =>
    switch (r) {
    | "open" => Some(`OPEN)
    | "closed" => Some(`JUDGED)
    | "pending" => Some(`JUDGEMENT_PENDING)
    | _ => None
    };

  let fromString = (r: string): query => {
    let keyValuePairs = KeyValuePairs.fromSearchParam(r);
    let _state =
      switch (keyValuePairs |> KeyValuePairs.get("state")) {
      | Some(state) => stateFromString(state)
      | _ => None
      };
    {state: _state};
  };
};

module SearchResultsA = {
  type query = {
    key: string,
    value: string,
  };
  type t = array(query);
};

let load3Queries = (channelId, states, itemsPerPage, fn) =>
  ((a, b, c) => (a, b, c) |> fn)
  |> E.F.flatten3Callbacks(
       Types.SelectWithPaginationReducer.make(
         ~itemsPerPage,
         ~callFnParams={channelId, states},
         ~subComponent=_,
       ),
       Queries.Channel.component2(~id=channelId),
       Queries.SeriesCollection.component2(~channelId),
     );

let make =
    (
      ~channelId: string,
      ~searchParams: string,
      ~loggedInUser: Context.Primary.User.t,
      ~itemsPerPage: int=20,
      ~layout,
    ) => {
  let states =
    SearchResults.fromString(searchParams).state
    |> E.O.fmap(r => [|r|])
    |> E.O.default([|`OPEN|]);
  let loadData = load3Queries(channelId, states, itemsPerPage);
  loadData(((selectWithPaginationParams, channel, query)) =>
    Types.MeasurableIndexDataState.make({
      reducerParams: selectWithPaginationParams,
      loggedInUser,
      channel,
      query,
    })
    |> Components.MeasurableIndexDataState.toLayoutInput(
         selectWithPaginationParams.send,
       )
    |> layout
  );
};