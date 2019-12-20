open Utils;

/**
 * @todo:  Using <Components /> style in React.js is preferable then
 * @todo:  functions. Try first of all to create new component.
 **/

let formatDate = e =>
  e |> E.O.fmap(E.M.format(E.M.format_simple)) |> E.O.default("");

module Styles = {
  open Css;
  let link =
    style([
      fontSize(`em(1.0)),
      marginRight(`px(18)),
      color(`hex("bbb")),
      selector(
        " a",
        [color(`hex("aaa")), selector(":hover", [color(hex("333"))])],
      ),
    ]);
  let link2 =
    style([
      fontSize(`em(1.0)),
      color(`hex("777")),
      selector(
        " a",
        [color(`hex("aaa")), selector(":hover", [color(hex("333"))])],
      ),
    ]);

  let nameLink =
    style([
      fontSize(`em(1.)),
      color(`hex("333")),
      selector(
        " a",
        [color(`hex("aaa")), selector(":hover", [color(hex("333"))])],
      ),
    ]);

  let dateOnStyle =
    style([
      marginLeft(`em(0.2)),
      lineHeight(`em(1.56)),
      fontSize(`em(1.1)),
    ]);

  let description = style([paddingTop(`em(1.5))]);
};

module DateItem = {
  [@react.component]
  let make =
      (~m: Types.measurable, ~showOn=true, ~onStyle=Styles.dateOnStyle, ()) =>
    switch (m.labelOnDate |> E.O.fmap(E.M.goFormat_simple), showOn) {
    | (None, _) => <Null />
    | (Some(e), true) =>
      [|
        <span className=onStyle key="on"> {"on " |> ste} </span>,
        <span className=Shared.TagLink.dateItem key="dateItem">
          {e |> ste}
        </span>,
      |]
      |> ReasonReact.array
    | (Some(e), false) =>
      <span className=Shared.TagLink.dateItem> {e |> ste} </span>
    };
};

module LinkName = {
  [@react.component]
  let make = (~m: Types.measurable) =>
    <Providers.AppContext.Consumer>
      ...{context => {
        module Config = {
          let globalSetting = context.globalSetting;
        };
        module Ken = KenTools.Functor(Config);
        module MeasurableEntityLinks = MeasurableEntityLinks.Functor(Ken);
        <>
          {MeasurableEntityLinks.nameEntityLink(
             ~m,
             ~className=Shared.TagLink.item,
           )
           |> E.O.React.defaultNull}
          {MeasurableEntityLinks.propertyEntityLink(
             ~m,
             ~className=Shared.TagLink.property,
           )
           |> E.O.React.defaultNull}
          {<DateItem m />}
        </>;
      }}
    </Providers.AppContext.Consumer>;
};

module LinkMeasurable = {
  [@react.component]
  let make = (~m: Types.measurable) => {
    switch (m.labelSubject, m.labelProperty) {
    | (Some(""), Some(""))
    | (None, _)
    | (_, None) =>
      <Link
        className=Styles.nameLink
        linkType={Internal(MeasurableShow(m.channelId, m.id))}>
        {m.name |> ste}
      </Link>
    | (Some(_), Some(_)) => <LinkName m />
    };
  };
};

module Description = {
  [@react.component]
  let make = (~m: Types.measurable) =>
    switch (m.labelCustom) {
    | Some("")
    | None => <Null />
    | Some(text) =>
      <FC__Div styles=[Styles.description]>
        <Markdown source=text />
      </FC__Div>
    };
};

module EndpointResponse = {
  [@react.component]
  let make = (~m: Types.measurable) =>
    switch (
      m.resolutionEndpoint |> E.O.default(""),
      m.resolutionEndpointResponse,
    ) {
    | ("", _) => <Null />
    | (_, Some(r)) =>
      "Current Endpoint Value: " ++ E.Float.with3DigitsPrecision(r) |> ste
    | _ => <Null />
    };
};

module QuestionLink = {
  [@react.component]
  let make = (~m: Types.measurable) =>
    <Link
      className=Shared.Item.item
      linkType={Internal(MeasurableShow(m.channelId, m.id))}>
      {"Link to This Question" |> Utils.ste}
    </Link>;
};

module CreatorLink = {
  [@react.component]
  let make = (~m: Types.measurable) =>
    m.creator
    |> E.O.fmap((agent: Types.agent) =>
         <div className=Shared.Item.item> <AgentLink agent /> </div>
       )
    |> E.O.default(<Null />);
};

module ChannelLink = {
  [@react.component]
  let make = (~m: Types.measurable) =>
    <div className=Shared.Item.item>
      <Link
        linkType={Internal(ChannelShow(m.channelId))}
        className=Shared.Item.item>
        {switch (m.channel) {
         | Some(channel) => "#" ++ channel.name |> ste
         | _ => "#channel" |> ste
         }}
      </Link>
    </div>;
};

module Measurements = {
  [@react.component]
  let make = (~m: Types.measurable) =>
    switch (m.measurementCount) {
    | Some(0) => <Null />
    | None => <Null />
    | Some(count) =>
      let popoverContent =
        "Number of Predictions: " ++ string_of_int(count) |> ste;
      <AntdPopover content=popoverContent trigger=`hover placement=`top>
        <div className=Shared.Item.item>
          <Icon.Icon icon="BULB" />
          {count |> string_of_int |> ste}
        </div>
      </AntdPopover>;
    };
};

module Measurers = {
  [@react.component]
  let make = (~m: Types.measurable) =>
    switch (m.measurerCount) {
    | Some(0) => <Null />
    | None => <Null />
    | Some(count) =>
      <div className=Shared.Item.item>
        <Icon.Icon icon="PEOPLE" />
        {count |> string_of_int |> ste}
      </div>
    };
};

module Id = {
  [@react.component]
  let make = (~m: Types.measurable) =>
    <Link
      className=Styles.link2
      linkType={Internal(MeasurableShow(m.channelId, m.id))}>
      {m.id |> Utils.ste}
    </Link>;
};

module Series = {
  [@react.component]
  let make = (~m: Types.measurable) => {
    m.series
    |> E.O.fmap((r: Types.series) =>
         switch (r.name) {
         | Some(name) =>
           <div className=Shared.Item.item>
             <Link linkType={Internal(SeriesShow(m.channelId, r.id))}>
               <Icon.Icon icon="LAYERS" />
               {name |> ste}
             </Link>
           </div>
         | None => <Null />
         }
       )
    |> E.O.default(<Null />);
  };
};

module ExpectedResolutionDate = {
  [@react.component]
  let make = (~m: Types.measurable) =>
    <div className=Shared.Item.item>
      {"Resolves on " ++ (m.expectedResolutionDate |> formatDate) |> ste}
    </div>;
};

module ResolutionEndpoint = {
  [@react.component]
  let make = (~m: Types.measurable) =>
    switch (m.resolutionEndpoint |> E.O.default("")) {
    | "" => <Null />
    | text =>
      <div className=Shared.Item.item> {"Endpoint: " ++ text |> ste} </div>
    };
};

module ArchiveButton = {
  [@react.component]
  let make = (~m: Types.measurable) =>
    <MeasurableArchive.Mutation>
      ...{(mutation, _) =>
        <div className=Shared.Item.item>
          <div
            className={Shared.Item.itemButton(DANGER)}
            onClick={e => {
              MeasurableArchive.mutate(mutation, m.id);
              ReactEvent.Synthetic.stopPropagation(e);
            }}>
            {"Archive" |> ste}
          </div>
        </div>
      }
    </MeasurableArchive.Mutation>;
};

module UnArchiveButton = {
  [@react.component]
  let make = (~m: Types.measurable) =>
    <MeasurableUnarchive.Mutation>
      ...{(mutation, _) =>
        <div className=Shared.Item.item>
          <div
            className={Shared.Item.itemButton(DANGER)}
            onClick={e => {
              MeasurableUnarchive.mutate(mutation, m.id);
              ReactEvent.Synthetic.stopPropagation(e);
            }}>
            {"Unarchive" |> ste}
          </div>
        </div>
      }
    </MeasurableUnarchive.Mutation>;
};

let archiveOption = (~m: Types.measurable) =>
  m.isArchived == Some(true) ? <UnArchiveButton m /> : <ArchiveButton m />;