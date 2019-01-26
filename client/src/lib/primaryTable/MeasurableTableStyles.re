open Utils;
open Rationale;
open PrimaryTableBase;

let compareSimilarMeasurables =
    (measurableA: DataModel.measurable, measurableB: DataModel.measurable) =>
  switch (
    measurableA.expectedResolutionDate,
    measurableB.expectedResolutionDate,
  ) {
  | (Some(a), Some(b)) => MomentRe.Moment.isAfter(a, b) ? 1 : (-1)
  | _ => 1
  };

let compareMeasurables =
    (measurableA: DataModel.measurable, measurableB: DataModel.measurable) =>
  switch (status(measurableA), status(measurableB)) {
  | (a, b) when a == b => compareSimilarMeasurables(measurableA, measurableB)
  | (a, b) => statusInt(a) > statusInt(b) ? (-1) : 1
  };

let formatDate = e =>
  Option.Infix.(e <$> MomentRe.Moment.format("L") |> Option.default(""));

let dateFinder = (head, p, date) => {
  let date = formatDate(date);
  <div className=PrimaryTableStyles.statusRow>
    <h3> {head |> ste} </h3>
    <p> {p ++ date |> ste} </p>
  </div>;
};

let dateStatus = (~measurable: DataModel.measurable) => {
  let m = measurable;
  switch (status(m)) {
  | OPEN => dateFinder("Open", "Closes ~", m.expectedResolutionDate)
  | PENDING_REVIEW =>
    dateFinder(
      "Judgement Pending",
      "Pending since ",
      m.expectedResolutionDate,
    )
  | ARCHIVED => dateFinder("Archived", "Archived on ", m.stateUpdatedAt)
  | JUDGED => dateFinder("Judged", "Judged on ", m.stateUpdatedAt)
  };
};

let dateStatusWrapper = (~measurable: DataModel.measurable) =>
  <div className={PrimaryTableStyles.statusColor(~measurable)}>
    {dateStatus(~measurable)}
  </div>;

let sortMeasurables = m => Belt.SortArray.stableSortBy(m, compareMeasurables);

let link = (~m: DataModel.measurable) =>
  <a href={"/measurables/" ++ m.id}> {m.name |> ste} </a>;

let description = (~m: DataModel.measurable) =>
  switch (m.description |> Option.default("")) {
  | "" => <div />
  | text => <p> {text |> ste} </p>
  };

let creatorLink = (~m: DataModel.measurable) =>
  <div className=PrimaryTableStyles.item>
    Option.Infix.(
      m.creator
      <$> (
        c =>
          <a href={"/agents/" ++ c.id}>
            {c.name |> Option.default("") |> ste}
          </a>
      )
      |> Option.default("" |> ste)
    )
  </div>;

let editLink = (~m: DataModel.measurable) =>
  <div className=PrimaryTableStyles.item>
    <a
      href={"/measurables/" ++ m.id ++ "/edit"}
      className={PrimaryTableStyles.itemButton(NORMAL)}>
      {"Edit" |> ste}
    </a>
  </div>;

let measurements = (~m: DataModel.measurable) =>
  switch (m.measurementCount) {
  | Some(0) => <div />
  | None => <div />
  | Some(count) =>
    <div className=PrimaryTableStyles.item>
      <span> {count |> string_of_int |> ste} </span>
      <span> {" measurements" |> ste} </span>
    </div>
  };

let measurers = (~m: DataModel.measurable) =>
  switch (m.measurerCount) {
  | Some(0) => <div />
  | None => <div />
  | Some(count) =>
    <div className=PrimaryTableStyles.item>
      <span>
        {count |> string_of_int |> ste}
        <span> {" measurers" |> ste} </span>
      </span>
    </div>
  };

let expectedResolutionDate = (~m: DataModel.measurable) =>
  <div className=PrimaryTableStyles.item>
    <span> {"Resolves on " |> ste} </span>
    <span> {m.expectedResolutionDate |> formatDate |> ste} </span>
  </div>;

let resolutionEndpoint = (~m: DataModel.measurable) =>
  switch (m.resolutionEndpoint |> Option.default("")) {
  | "" => <div />
  | text =>
    <div className=PrimaryTableStyles.item>
      <span> <span> {"Endpoint: " |> ste} </span> {text |> ste} </span>
    </div>
  };

module WithArchiveMutation = {
  module GraphQL = [%graphql
    {|
             mutation archiveMeasurable($id: String!) {
                 archiveMeasurable(id: $id) {
                   id
                 }
             }
     |}
  ];

  module Mutation = ReasonApollo.CreateMutation(GraphQL);

  let mutate = (mutation: Mutation.apolloMutation, id: string) => {
    let m = GraphQL.make(~id, ());
    mutation(
      ~variables=m##variables,
      ~refetchQueries=[|"getAgent", "getMeasurables"|],
      (),
    )
    |> ignore;
  };
};

module WithUnarchiveMutation = {
  module GraphQL = [%graphql
    {|
             mutation unArchiveMeasurable($id: String!) {
                 unArchiveMeasurable(id: $id) {
                   id
                 }
             }
     |}
  ];

  module Mutation = ReasonApollo.CreateMutation(GraphQL);

  let mutate = (mutation: Mutation.apolloMutation, id: string) => {
    let m = GraphQL.make(~id, ());
    mutation(
      ~variables=m##variables,
      ~refetchQueries=[|"getAgent", "getMeasurables"|],
      (),
    )
    |> ignore;
  };
};

let archiveButton = (~m: DataModel.measurable) =>
  WithArchiveMutation.Mutation.make((mutation, _) =>
    <div className=PrimaryTableStyles.item>
      <div
        className={PrimaryTableStyles.itemButton(DANGER)}
        onClick={_ => WithArchiveMutation.mutate(mutation, m.id)}>
        {"Archive" |> ste}
      </div>
    </div>
  )
  |> ReasonReact.element;

let unArchiveButton = (~m: DataModel.measurable) =>
  WithUnarchiveMutation.Mutation.make((mutation, _) =>
    <div className=PrimaryTableStyles.item>
      <div
        className={PrimaryTableStyles.itemButton(DANGER)}
        onClick={_ => WithUnarchiveMutation.mutate(mutation, m.id)}>
        {"Unarchive" |> ste}
      </div>
    </div>
  )
  |> ReasonReact.element;

let archiveOption = (~m: DataModel.measurable) =>
  status(m) !== ARCHIVED ? archiveButton(~m) : unArchiveButton(~m);