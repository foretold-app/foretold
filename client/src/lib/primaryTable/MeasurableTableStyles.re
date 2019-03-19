open Utils;
open PrimaryTableBase;

let compareSimilarMeasurables =
    (measurableA: DataModel.Measurable.t, measurableB: DataModel.Measurable.t) =>
  switch (
    measurableA.expectedResolutionDate,
    measurableB.expectedResolutionDate,
  ) {
  | (Some(a), Some(b)) => MomentRe.Moment.isAfter(a, b) ? 1 : (-1)
  | _ => 1
  };

let compareMeasurables =
    (measurableA: DataModel.Measurable.t, measurableB: DataModel.Measurable.t) =>
  switch (status(measurableA), status(measurableB)) {
  | (a, b) when a == b => compareSimilarMeasurables(measurableA, measurableB)
  | (a, b) => statusInt(a) > statusInt(b) ? (-1) : 1
  };

let formatDate = e =>
  e |> E.O.fmap(E.M.format(E.M.format_simple)) |> E.O.default("");

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

let dateStatusI = (~measurable: DataModel.Measurable.t, ~dateDisplay) => {
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

let dateStatus = (~measurable: DataModel.Measurable.t) =>
  dateStatusI(~measurable, ~dateDisplay=WHOLE);

let dateStatusWrapper = (~measurable: DataModel.Measurable.t) =>
  <div className={PrimaryTableStyles.statusColor(~measurable)}>
    {dateStatus(~measurable)}
  </div>;

let sortMeasurables = m => E.A.stableSortBy(m, compareMeasurables);

let graph = Data.make;

let description = (description: string) =>
  ItemShow.findName(graph, description);

let nameWithDate = (~m: DataModel.Measurable.t) =>
  switch (formatDate(m.descriptionDate)) {
  | "" => m.name
  | e => m.name ++ " on " ++ e
  };

let itemUrl = id => {j|/items/$id|j};

let xEntityLink = (attribute, ~m: DataModel.Measurable.t, ~className: string) =>
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

let link = (~m: DataModel.Measurable.t) =>
  <>
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
      | "" => E.React.null
      | e =>
        [|
          <span className=PrimaryTableStyles.calDate> {"on " |> ste} </span>,
          <span className=PrimaryTableStyles.calDateO> {e |> ste} </span>,
        |]
        |> ReasonReact.array
      }
    }
  </>;

let description = (~m: DataModel.Measurable.t) =>
  switch (m.description) {
  | Some("")
  | None => E.React.null
  | Some(text) => <p> {text |> ste} </p>
  };

/* TODO: Move */
let stringOfFloat = Js.Float.toPrecisionWithPrecision(_, ~digits=3);

let endpointResponse = (~m: DataModel.Measurable.t) =>
  switch (
    m.resolutionEndpoint |> E.O.default(""),
    m.resolutionEndpointResponse,
  ) {
  | ("", _) => E.React.null
  | (_, Some(r)) => "Current Endpoint Value: " ++ stringOfFloat(r) |> ste
  | _ => E.React.null
  };

let creatorLink = (~m: DataModel.Measurable.t) =>
  m.creator
  |> E.O.fmap((c: DataModel.Agent.t) =>
       <div className=PrimaryTableStyles.item>
         <a href={DataModel.Url.toString(AgentShow(c.id))}>
           {c.name |> E.O.default("") |> ste}
         </a>
       </div>
     )
  |> E.O.React.defaultNull;

let editLink = (~m: DataModel.Measurable.t) =>
  <div className=PrimaryTableStyles.item>
    <a
      href={DataModel.Url.toString(MeasurableEdit(m.id))}
      className={PrimaryTableStyles.itemButton(NORMAL)}>
      {"Edit" |> ste}
    </a>
  </div>;

let measurements = (~m: DataModel.Measurable.t) =>
  switch (m.measurementCount) {
  | Some(0) => <div />
  | None => <div />
  | Some(count) =>
    <div className=PrimaryTableStyles.item>
      <Icon.Icon icon="BULB" />
      <span> {count |> string_of_int |> ste} </span>
    </div>
  };

let measurers = (~m: DataModel.Measurable.t) =>
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

let series = (~m: DataModel.Measurable.t) =>
  m.series
  |> E.O.bind(_, r =>
       switch (r.name) {
       | Some(name) =>
         Some(
           <div className=PrimaryTableStyles.item>
             <Icon.Icon icon="LAYERS" />
             <a
               href={
                 DataModel.Url.toString(
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

let expectedResolutionDate = (~m: DataModel.Measurable.t) =>
  <div className=PrimaryTableStyles.item>
    <span> {"Resolves on " |> ste} </span>
    <span> {m.expectedResolutionDate |> formatDate |> ste} </span>
  </div>;

let resolutionEndpoint = (~m: DataModel.Measurable.t) =>
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

let archiveButton = (~m: DataModel.Measurable.t) =>
  WithArchiveMutation.Mutation.make((mutation, _) =>
    <div className=PrimaryTableStyles.item>
      <div
        className={PrimaryTableStyles.itemButton(DANGER)}
        onClick={_ => WithArchiveMutation.mutate(mutation, m.id)}>
        {"Archive" |> ste}
      </div>
    </div>
  )
  |> E.React.el;

let unArchiveButton = (~m: DataModel.Measurable.t) =>
  WithUnarchiveMutation.Mutation.make((mutation, _) =>
    <div className=PrimaryTableStyles.item>
      <div
        className={PrimaryTableStyles.itemButton(DANGER)}
        onClick={_ => WithUnarchiveMutation.mutate(mutation, m.id)}>
        {"Unarchive" |> ste}
      </div>
    </div>
  )
  |> E.React.el;

let archiveOption = (~m: DataModel.Measurable.t) =>
  status(m) !== ARCHIVED ? archiveButton(~m) : unArchiveButton(~m);