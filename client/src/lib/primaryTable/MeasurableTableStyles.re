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
  Option.Infix.(e <$> MomentRe.Moment.format("L") |> E.O.default(""));

type dateDisplay =
  | TOP
  | BOTTOM
  | WHOLE;

let dateFinder = (head, p, date, dateDisplay) => {
  let date = formatDate(date);
  switch (dateDisplay) {
  | TOP => head |> ste
  | BOTTOM => p ++ date |> ste
  | WHOLE =>
    <div className=PrimaryTableStyles.statusRow>
      <h3> {head |> ste} </h3>
      <p> {p ++ date |> ste} </p>
    </div>
  };
};

let dateStatusI = (~measurable: DataModel.measurable, ~dateDisplay) => {
  let m = measurable;
  switch (status(m)) {
  | OPEN =>
    dateFinder("Open", "Closes ~", m.expectedResolutionDate, dateDisplay)
  | PENDING_REVIEW =>
    dateFinder(
      "Judgement Pending",
      "Pending since ",
      m.expectedResolutionDate,
      dateDisplay,
    )
  | ARCHIVED =>
    dateFinder("Archived", "Archived on ", m.stateUpdatedAt, dateDisplay)
  | JUDGED =>
    dateFinder("Judged", "Judged on ", m.stateUpdatedAt, dateDisplay)
  };
};

let dateStatus = (~measurable: DataModel.measurable) =>
  dateStatusI(~measurable, ~dateDisplay=WHOLE);

let dateStatusWrapper = (~measurable: DataModel.measurable) =>
  <div className={PrimaryTableStyles.statusColor(~measurable)}>
    {dateStatus(~measurable)}
  </div>;

let sortMeasurables = m => E.A.stableSortBy(m, compareMeasurables);

let graph = Data.make;

let description = (description: string) =>
  ItemShow.findName(graph, description);

let nameWithDate = (~m: DataModel.measurable) =>
  switch (formatDate(m.descriptionDate)) {
  | "" => m.name
  | e => m.name ++ " on " ++ e
  };

let itemUrl = id => {j|/items/$id|j};

let xEntityLink = (attribute, ~m: DataModel.measurable, ~className: string) =>
  m
  |> attribute
  |> E.O.bind(_, ItemShow.findName(graph))
  |> E.O.bind(_, r =>
       m
       |> attribute
       |> E.O.fmap(d => <a href={d |> itemUrl} className> {r |> ste} </a>)
     );

let nameEntityLink = xEntityLink(r => r.descriptionEntity);
let propertyEntityLink = xEntityLink(r => r.descriptionProperty);

let link = (~m: DataModel.measurable) =>
  <div>
    {
      nameEntityLink(~m, ~className=PrimaryTableStyles.itemLink)
      |> E.O.React.defaultNull
    }
    {
      propertyEntityLink(~m, ~className=PrimaryTableStyles.propertyLink)
      |> E.O.React.defaultNull
    }
    <span className=PrimaryTableStyles.namme> {m.name |> ste} </span>
    {
      switch (formatDate(m.descriptionDate)) {
      | "" => ReasonReact.null
      | e =>
        [|
          <span className=PrimaryTableStyles.calDate> {"on " |> ste} </span>,
          <span className=PrimaryTableStyles.calDateO> {e |> ste} </span>,
        |]
        |> ReasonReact.array
      }
    }
  </div>;

let description = (~m: DataModel.measurable) =>
  switch (m.description) {
  | Some("")
  | None => ReasonReact.null
  | Some(text) => <p> {text |> ste} </p>
  };

/* TODO: Move */
let stringOfFloat = Js.Float.toPrecisionWithPrecision(_, ~digits=3);

let endpointResponse = (~m: DataModel.measurable) =>
  switch (
    m.resolutionEndpoint |> E.O.default(""),
    m.resolutionEndpointResponse,
  ) {
  | ("", _) => ReasonReact.null
  | (_, Some(r)) => "Current Endpoint Value: " ++ stringOfFloat(r) |> ste
  | _ => ReasonReact.null
  };

let creatorLink = (~m: DataModel.measurable) =>
  <div className=PrimaryTableStyles.item>
    Option.Infix.(
      m.creator
      <$> (
        c =>
          <a href={Urls.mapLinkToUrl(AgentShow(c.id))}>
            {c.name |> E.O.default("") |> ste}
          </a>
      )
      |> E.O.default("" |> ste)
    )
  </div>;

let editLink = (~m: DataModel.measurable) =>
  <div className=PrimaryTableStyles.item>
    <a
      href={Urls.mapLinkToUrl(MeasurableEdit(m.id))}
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
      <Icon.Icon icon="BULB" />
      <span> {count |> string_of_int |> ste} </span>
    </div>
  };

let measurers = (~m: DataModel.measurable) =>
  switch (m.measurerCount) {
  | Some(0) => <div />
  | None => <div />
  | Some(count) =>
    <div className=PrimaryTableStyles.item>
      <span>
        <Icon.Icon icon="PEOPLE" />
        {count |> string_of_int |> ste}
      </span>
    </div>
  };

let series = (~m: DataModel.measurable) =>
  m.series
  |> Option.bind(_, r =>
       switch (r.name) {
       | Some(name) =>
         Some(
           <div className=PrimaryTableStyles.item>
             <Icon.Icon icon="LAYERS" />
             <a
               href={
                 Urls.mapLinkToUrl(
                   SeriesShow(m.channel |> E.O.default(""), r.id),
                 )
               }>
               {name |> ste}
             </a>
           </div>,
         )
       | None => None
       }
     )
  |> E.O.React.defaultNull;

let expectedResolutionDate = (~m: DataModel.measurable) =>
  <div className=PrimaryTableStyles.item>
    <span> {"Resolves on " |> ste} </span>
    <span> {m.expectedResolutionDate |> formatDate |> ste} </span>
  </div>;

let resolutionEndpoint = (~m: DataModel.measurable) =>
  switch (m.resolutionEndpoint |> E.O.default("")) {
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