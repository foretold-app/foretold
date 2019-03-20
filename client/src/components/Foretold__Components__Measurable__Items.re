open Utils;

module Shared = Foretold__Components__Shared;
let formatDate = e =>
  e |> E.O.fmap(E.M.format(E.M.format_simple)) |> E.O.default("");

type dateDisplay =
  | TOP
  | BOTTOM
  | WHOLE;

let sortMeasurables = m => E.A.stableSortBy(m, DataModel.Measurable.compare);

module Entities = {
  let graph = Data.make;

  let description = (description: string) =>
    ItemShow.findName(graph, description);

  let nameWithDate = (~m: DataModel.Measurable.t) =>
    switch (formatDate(m.descriptionDate)) {
    | "" => m.name
    | e => m.name ++ " on " ++ e
    };

  let itemUrl = id => {j|/items/$id|j};

  let xEntityLink =
      (attribute, ~m: DataModel.Measurable.t, ~className: string) =>
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
};

let link = (~m: DataModel.Measurable.t) =>
  <>
    {
      Entities.nameEntityLink(~m, ~className=PrimaryTableStyles.itemLink)
      |> E.O.React.defaultNull
    }
    {
      Entities.propertyEntityLink(
        ~m,
        ~className=PrimaryTableStyles.propertyLink,
      )
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
       <div className=Shared.Item.item>
         <a href={DataModel.Url.toString(AgentShow(c.id))}>
           {c.name |> E.O.default("") |> ste}
         </a>
       </div>
     )
  |> E.O.React.defaultNull;

let editLink = (~m: DataModel.Measurable.t) =>
  <div className=Shared.Item.item>
    <a
      href={DataModel.Url.toString(MeasurableEdit(m.id))}
      className={Shared.Item.itemButton(NORMAL)}>
      {"Edit" |> ste}
    </a>
  </div>;

let measurements = (~m: DataModel.Measurable.t) =>
  switch (m.measurementCount) {
  | Some(0) => <div />
  | None => <div />
  | Some(count) =>
    <div className=Shared.Item.item>
      <Icon.Icon icon="BULB" />
      <span> {count |> string_of_int |> ste} </span>
    </div>
  };

let measurers = (~m: DataModel.Measurable.t) =>
  switch (m.measurerCount) {
  | Some(0) => <div />
  | None => <div />
  | Some(count) =>
    <div className=Shared.Item.item>
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
           <div className=Shared.Item.item>
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
  <div className=Shared.Item.item>
    <span> {"Resolves on " |> ste} </span>
    <span> {m.expectedResolutionDate |> formatDate |> ste} </span>
  </div>;

let resolutionEndpoint = (~m: DataModel.Measurable.t) =>
  switch (m.resolutionEndpoint |> E.O.default("")) {
  | "" => ReasonReact.null
  | text =>
    <div className=Shared.Item.item>
      <span> <span> {"Endpoint: " |> ste} </span> {text |> ste} </span>
    </div>
  };

let archiveButton = (~m: DataModel.Measurable.t) =>
  Foretold__GraphQL.Mutations.MeasurableUnarchive.Mutation.make((mutation, _) =>
    <div className=Shared.Item.item>
      <div
        className={Shared.Item.itemButton(DANGER)}
        onClick={
          _ =>
            Foretold__GraphQL.Mutations.MeasurableUnarchive.mutate(
              mutation,
              m.id,
            )
        }>
        {"Archive" |> ste}
      </div>
    </div>
  )
  |> E.React.el;

let unArchiveButton = (~m: DataModel.Measurable.t) =>
  Foretold__GraphQL.Mutations.MeasurableArchive.Mutation.make((mutation, _) =>
    <div className=Shared.Item.item>
      <div
        className={Shared.Item.itemButton(DANGER)}
        onClick={
          _ =>
            Foretold__GraphQL.Mutations.MeasurableArchive.mutate(
              mutation,
              m.id,
            )
        }>
        {"Unarchive" |> ste}
      </div>
    </div>
  )
  |> E.React.el;

let archiveOption = (~m: DataModel.Measurable.t) =>
  DataModel.Measurable.toStatus(m) !== ARCHIVED ?
    archiveButton(~m) : unArchiveButton(~m);