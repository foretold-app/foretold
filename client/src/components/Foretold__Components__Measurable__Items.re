open Utils;
open DataModel;

module Shared = Foretold__Components__Shared;

type measurable = Measurable.t;

module MeasurableEntityLinks = {
  let xEntityLink = (attribute, ~m: measurable, ~className: string) =>
    m
    |> attribute
    |> E.O.bind(_, Foretold__Components__Ken.findName)
    |> E.O.bind(_, r =>
         m
         |> attribute
         |> E.O.fmap(d =>
              <a href={d |> Foretold__Components__Ken.itemUrl} className>
                {r |> ste}
              </a>
            )
       );

  let nameEntityLink = xEntityLink(r => r.descriptionEntity);
  let propertyEntityLink = xEntityLink(r => r.descriptionProperty);
};

let formatDate = e =>
  e |> E.O.fmap(E.M.format(E.M.format_simple)) |> E.O.default("");

let dateOnStyle =
  Css.(
    style([
      marginLeft(`em(0.3)),
      lineHeight(`em(1.56)),
      fontSize(`em(1.1)),
    ])
  );

let dateItem = (~m: measurable, ~showOn=true, ~onStyle=dateOnStyle, ()) =>
  switch (m.descriptionDate |> E.O.fmap(E.M.goFormat_simple), showOn) {
  | (None, _) => None
  | (Some(e), true) =>
    Some(
      [|
        <span className=onStyle> {"on " |> ste} </span>,
        <span className=Shared.TagLink.dateItem> {e |> ste} </span>,
      |]
      |> ReasonReact.array,
    )
  | (Some(e), false) =>
    Some(<span className=Shared.TagLink.dateItem> {e |> ste} </span>)
  };

let link = (~m: measurable) => {
  open Css;
  let name = style([fontSize(`em(1.2)), color(`hex("333"))]);
  <>
    {
      MeasurableEntityLinks.nameEntityLink(~m, ~className=Shared.TagLink.item)
      |> E.O.React.defaultNull
    }
    {
      MeasurableEntityLinks.propertyEntityLink(
        ~m,
        ~className=Shared.TagLink.property,
      )
      |> E.O.React.defaultNull
    }
    <span className=name> {m.name |> ste} </span>
    {dateItem(~m, ()) |> E.O.React.defaultNull}
  </>;
};

let description = (~m: measurable) =>
  switch (m.description) {
  | Some("")
  | None => None
  | Some(text) => Some(text |> ste |> E.React.inP)
  };

let endpointResponse = (~m: measurable) =>
  switch (
    m.resolutionEndpoint |> E.O.default(""),
    m.resolutionEndpointResponse,
  ) {
  | ("", _) => None
  | (_, Some(r)) =>
    Some(
      "Current Endpoint Value: " ++ E.Float.with3DigitsPrecision(r) |> ste,
    )
  | _ => None
  };

let creatorLink = (~m: measurable) =>
  m.creator
  |> E.O.fmap((c: Agent.t) =>
       <div className=Shared.Item.item>
         <a href={Context.Routing.Url.toString(AgentShow(c.id))}>
           {c.name |> E.O.default("") |> ste}
         </a>
       </div>
     );

let editLink = (~m: measurable) =>
  <div className=Shared.Item.item>
    <a
      href={Context.Routing.Url.toString(MeasurableEdit(m.id))}
      className={Shared.Item.itemButton(NORMAL)}>
      {"Edit" |> ste}
    </a>
  </div>;

let measurements = (~m: measurable) =>
  switch (m.measurementCount) {
  | Some(0) => None
  | None => None
  | Some(count) =>
    Some(
      <div className=Shared.Item.item>
        <Icon.Icon icon="BULB" />
        {count |> string_of_int |> ste}
      </div>,
    )
  };

let measurers = (~m: measurable) =>
  switch (m.measurerCount) {
  | Some(0) => None
  | None => None
  | Some(count) =>
    Some(
      <div className=Shared.Item.item>
        <Icon.Icon icon="PEOPLE" />
        {count |> string_of_int |> ste}
      </div>,
    )
  };

let series = (~m: measurable) =>
  m.series
  |> E.O.bind(_, r =>
       switch (r.name) {
       | Some(name) =>
         Some(
           <div className=Shared.Item.item>
             <Icon.Icon icon="LAYERS" />
             <a
               href={
                 Context.Routing.Url.toString(
                   SeriesShow(m.channel |> E.O.default(""), r.id),
                 )
               }>
               {name |> ste}
             </a>
           </div>,
         )
       | None => None
       }
     );

let expectedResolutionDate = (~m: measurable) =>
  <div className=Shared.Item.item>
    {"Resolves on " ++ (m.expectedResolutionDate |> formatDate) |> ste}
  </div>;

let resolutionEndpoint = (~m: measurable) =>
  switch (m.resolutionEndpoint |> E.O.default("")) {
  | "" => None
  | text =>
    Some(
      <div className=Shared.Item.item> {"Endpoint: " ++ text |> ste} </div>,
    )
  };

let archiveButton = (~m: measurable) =>
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

let unArchiveButton = (~m: measurable) =>
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

let archiveOption = (~m: measurable) =>
  Measurable.toStatus(m) !== ARCHIVED ?
    archiveButton(~m) : unArchiveButton(~m);