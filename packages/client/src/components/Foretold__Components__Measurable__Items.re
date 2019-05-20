open Utils;
open Context.Primary;

module Shared = Foretold__Components__Shared;

type measurable = Measurable.t;

module Styles = {
  open Css;
  let tooltip =
    Css.style([
      Css.backgroundColor(`hex("f5f7f9")),
      Css.padding2(~v=`px(10), ~h=`px(13)),
      Css.border(`px(1), `solid, `hex("e8f2f9")),
      Css.borderRadius(`px(3)),
      Css.marginBottom(`px(10)),
    ]);
};

module MeasurableEntityLinks = {
  let xEntityLink = (attribute, ~m: measurable, ~className: string) =>
    m
    |> attribute
    |> E.O.bind(_, Foretold__Components__Ken.findName)
    |> E.O.bind(_, r =>
         m
         |> attribute
         |> E.O.fmap(d =>
              <Foretold__Components__Link
                linkType={External(d |> Foretold__Components__Ken.itemUrl)}
                className>
                {r |> ste}
              </Foretold__Components__Link>
            )
       );

  let nameEntityLink = xEntityLink(r => r.labelSubject);
  let propertyEntityLink = xEntityLink(r => r.labelProperty);
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
  switch (m.labelOnDate |> E.O.fmap(E.M.goFormat_simple), showOn) {
  | (None, _) => None
  | (Some(e), true) =>
    Some(
      [|
        <span className=onStyle key="on"> {"on " |> ste} </span>,
        <span className=Shared.TagLink.dateItem key="dateItem">
          {e |> ste}
        </span>,
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
    {MeasurableEntityLinks.nameEntityLink(~m, ~className=Shared.TagLink.item)
     |> E.O.React.defaultNull}
    {MeasurableEntityLinks.propertyEntityLink(
       ~m,
       ~className=Shared.TagLink.property,
     )
     |> E.O.React.defaultNull}
    <span className=name> {m.name |> ste} </span>
    {dateItem(~m, ()) |> E.O.React.defaultNull}
  </>;
};

let description = (~m: measurable) =>
  switch (m.labelCustom) {
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
       <Foretold__Components__Link
         linkType={Internal(AgentShow(c.id))} className=Shared.Item.item>
         {c.name |> E.O.default("") |> ste}
       </Foretold__Components__Link>
     );

let editLink = (~m: measurable) =>
  <div className=Shared.Item.item>
    <Foretold__Components__Link
      linkType={Internal(MeasurableEdit(m.id))}
      className={Shared.Item.itemButton(NORMAL)}>
      {"Edit" |> ste}
    </Foretold__Components__Link>
  </div>;

let measurements = (~m: measurable) =>
  switch (m.measurementCount) {
  | Some(0) => None
  | None => None
  | Some(count) =>
    Some(
      <div className=Shared.Item.item>
        <ReactPopover_Manager>
          <ReactPopover_Popper placement="auto">
            (
              (props: ReactPopover_Popper.popperProps) =>
                E.withDataAttributes(
                  ~data=[("data-placement", props##placement)],
                  <div ref=props##ref style=props##style>
                    <div className=Styles.tooltip>
                      {"Count " ++ string_of_int(count) |> ste}
                    </div>
                    <div
                      ref=props##arrowProps##ref
                      style=props##arrowProps##style
                    />
                  </div>,
                )
            )
          </ReactPopover_Popper>
          <ReactPopover_Reference>
            (
              (props: ReactPopover_Reference.referenceProps) =>
                <div ref=props##ref>
                  <Icon.Icon icon="BULB" />
                  {count |> string_of_int |> ste}
                </div>
            )
          </ReactPopover_Reference>
        </ReactPopover_Manager>
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
             <Foretold__Components__Link
               linkType={
                 Internal(SeriesShow(m.channel |> E.O.default(""), r.id))
               }>
               <Icon.Icon icon="LAYERS" />
               {name |> ste}
             </Foretold__Components__Link>
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
  Foretold__GraphQL.Mutations.MeasurableArchive.Mutation.make((mutation, _) =>
    <div className=Shared.Item.item>
      <div
        className={Shared.Item.itemButton(DANGER)}
        onClick={e => {
          Foretold__GraphQL.Mutations.MeasurableArchive.mutate(
            mutation,
            m.id,
          );
          ReactEvent.Synthetic.stopPropagation(e);
        }}>
        {"Archive" |> ste}
      </div>
    </div>
  )
  |> E.React.el;

let unArchiveButton = (~m: measurable) =>
  Foretold__GraphQL.Mutations.MeasurableUnarchive.Mutation.make((mutation, _) =>
    <div className=Shared.Item.item>
      <div
        className={Shared.Item.itemButton(DANGER)}
        onClick={e => {
          Foretold__GraphQL.Mutations.MeasurableUnarchive.mutate(
            mutation,
            m.id,
          );
          ReactEvent.Synthetic.stopPropagation(e);
        }}>
        {"Unarchive" |> ste}
      </div>
    </div>
  )
  |> E.React.el;

let archiveOption = (~m: measurable) =>
  m.isArchived == Some(true) ? unArchiveButton(~m) : archiveButton(~m);