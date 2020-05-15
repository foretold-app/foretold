open Utils;

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

  let conditionalOnStyle =
    style([
      marginLeft(`em(0.2)),
      lineHeight(`em(1.56)),
      fontSize(`em(1.1)),
    ]);
};

module DateItem = {
  [@react.component]
  let make =
      (
        ~measurable: Types.measurable,
        ~showOn=true,
        ~onStyle=Styles.dateOnStyle,
        (),
      ) =>
    switch (measurable.labelOnDate |> E.O.fmap(E.M.goFormat_simple), showOn) {
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

module StartEndDates = {
  [@react.component]
  let make = (~measurable: Types.measurable, ~onStyle=Styles.dateOnStyle, ()) => {
    let startAt =
      switch (measurable.labelStartAtDate) {
      | Some(e) => [|
          <span className=onStyle key="from"> {"from " |> ste} </span>,
          <span className=Shared.TagLink.dateItem key="fromItem">
            {e |> E.M.goFormat_short |> ste}
          </span>,
        |]
      | _ => [||]
      };
    let endAt =
      switch (measurable.labelEndAtDate) {
      | Some(e) => [|
          <span className=onStyle key="to"> {"to " |> ste} </span>,
          <span className=Shared.TagLink.dateItem key="toItem">
            {e |> E.M.goFormat_short |> ste}
          </span>,
        |]
      | _ => [||]
      };

    E.A.concatMany([|startAt, endAt|]) |> ReasonReact.array;
  };
};

module ConditionalOn = {
  [@react.component]
  let make =
      (~measurable: Types.measurable, ~onStyle=Styles.conditionalOnStyle, ()) => {
    let g = KenTools.Graph.fromContext();
    switch (measurable.labelConditionals) {
    | None => <Null />
    | Some(e) =>
      <>
        <span className=onStyle key="conditionalOn">
          {"conditional on " |> ste}
        </span>
        <span className=Shared.TagLink.conditionalOn key="conditionalOn">
          {MeasurableEntityLinks.conditionalOnEntityLinks(
             ~g,
             ~m=measurable,
             ~className=Shared.TagLink.item,
           )}
        </span>
      </>
    };
  };
};

module LinkName = {
  [@react.component]
  let make = (~measurable: Types.measurable) => {
    let g = KenTools.Graph.fromContext();
    <>
      {MeasurableEntityLinks.nameEntityLink(
         ~g,
         ~m=measurable,
         ~className=Shared.TagLink.item,
       )
       |> E.O.React.defaultNull}
      {MeasurableEntityLinks.propertyEntityLink(
         ~g,
         ~m=measurable,
         ~className=Shared.TagLink.property,
       )
       |> E.O.React.defaultNull}
      <DateItem measurable />
    </>;
  };
};

module LinkMeasurable = {
  [@react.component]
  let make = (~measurable: Types.measurable) => {
    <>
      {switch (measurable.labelSubject, measurable.labelProperty) {
       | (Some(""), Some(""))
       | (None, _)
       | (_, None) =>
         <Link
           className=Styles.nameLink
           linkType={
             Internal(MeasurableShow(measurable.channelId, measurable.id))
           }>
           {measurable.name |> ste}
         </Link>
       | (Some(_), Some(_)) => <LinkName measurable />
       }}
      <ConditionalOn measurable />
      <StartEndDates measurable />
    </>;
  };
};

module NameMeasurable = {
  [@react.component]
  let make = (~measurable: Types.measurable) => {
    <>
      {switch (measurable.labelSubject, measurable.labelProperty) {
       | (Some(""), Some(""))
       | (None, _)
       | (_, None) =>
         <span className=Styles.nameLink> {measurable.name |> ste} </span>
       | (Some(_), Some(_)) => <LinkName measurable />
       }}
      <ConditionalOn measurable />
      <StartEndDates measurable />
    </>;
  };
};

module Description = {
  [@react.component]
  let make = (~measurable: Types.measurable, ~styles=[]) =>
    switch (measurable.labelCustom) {
    | Some("")
    | None => <Null />
    | Some(text) =>
      <ForetoldComponents.Div styles>
        <Markdown source=text />
      </ForetoldComponents.Div>
    };
};

module EndpointResponse = {
  [@react.component]
  let make = (~measurable: Types.measurable) =>
    switch (
      measurable.resolutionEndpoint |> E.O.default(""),
      measurable.resolutionEndpointResponse,
    ) {
    | ("", _) => <Null />
    | (_, Some(r)) =>
      "Current Endpoint Value: " ++ E.Float.with3DigitsPrecision(r) |> ste
    | _ => <Null />
    };
};

module QuestionLink = {
  [@react.component]
  let make = (~measurable: Types.measurable) =>
    <Link
      className=Shared.Item.item
      linkType={
        Internal(MeasurableShow(measurable.channelId, measurable.id))
      }>
      {"Link to This Question" |> Utils.ste}
    </Link>;
};

module CreatorLink = {
  [@react.component]
  let make = (~measurable: Types.measurable) =>
    measurable.creator
    |> E.O.fmap((agent: Types.agent) =>
         <div className=Shared.Item.item> <AgentLink agent /> </div>
       )
    |> E.O.default(<Null />);
};

module ChannelLink = {
  [@react.component]
  let make = (~measurable: Types.measurable) =>
    <div className=Shared.Item.item>
      <Link
        linkType={Internal(ChannelShow(measurable.channelId))}
        className=Shared.Item.item>
        {switch (measurable.channel) {
         | Some(channel) => "#" ++ channel.name |> ste
         | _ => "#channel" |> ste
         }}
      </Link>
    </div>;
};

module Measurements = {
  [@react.component]
  let make = (~measurable: Types.measurable) =>
    switch (measurable.measurementCount) {
    | Some(0) => <Null />
    | None => <Null />
    | Some(count) =>
      let popoverContent =
        "Number of Predictions: " ++ string_of_int(count) |> ste;
      <Antd_Popover content=popoverContent trigger=`hover placement=`top>
        <div className=Shared.Item.item>
          <Icon icon="BULB" />
          {count |> string_of_int |> ste}
        </div>
      </Antd_Popover>;
    };
};

module Measurers = {
  [@react.component]
  let make = (~measurable: Types.measurable) =>
    switch (measurable.measurerCount) {
    | Some(0) => <Null />
    | None => <Null />
    | Some(count) =>
      <div className=Shared.Item.item>
        <Icon icon="PEOPLE" />
        {count |> string_of_int |> ste}
      </div>
    };
};

module Id = {
  [@react.component]
  let make = (~measurable: Types.measurable) =>
    <>
      <Link
        className=Styles.link2
        linkType={
          Internal(MeasurableShow(measurable.channelId, measurable.id))
        }>
        {measurable.id |> Utils.ste}
      </Link>
      <CopyToClipboard
        text={measurable.id}
        onCopy={_ =>
          Antd_Notification.info({
            "message": Lang.copiedMessage |> Utils.ste,
            "description": Lang.copiedDescription |> Utils.ste,
          })
        }>
        <Antd.Button icon=Antd.IconName.copy shape=`circle _type=`link />
      </CopyToClipboard>
    </>;
};

module Series = {
  [@react.component]
  let make = (~measurable: Types.measurable) => {
    measurable.series
    |> E.O.fmap((r: Types.series) =>
         switch (r.name) {
         | Some(name) =>
           <div className=Shared.Item.item>
             <Link
               linkType={Internal(SeriesShow(measurable.channelId, r.id))}>
               <Icon icon="LAYERS" />
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
  let make = (~measurable: Types.measurable) =>
    <div className=Shared.Item.item>
      {"Resolves on "
       ++ (measurable.expectedResolutionDate |> formatDate)
       |> ste}
    </div>;
};

module ResolutionEndpoint = {
  [@react.component]
  let make = (~measurable: Types.measurable) =>
    switch (measurable.resolutionEndpoint |> E.O.default("")) {
    | "" => <Null />
    | text =>
      <div className=Shared.Item.item> {"Endpoint: " ++ text |> ste} </div>
    };
};

module ArchiveButton = {
  [@react.component]
  let make = (~measurable: Types.measurable) =>
    <MeasurableArchive.Mutation>
      ...{(mutation, _) =>
        <div className=Shared.Item.item>
          <div
            className={Shared.Item.itemButton(DANGER)}
            onClick={e => {
              MeasurableArchive.mutate(mutation, measurable.id);
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
  let make = (~measurable: Types.measurable) =>
    <MeasurableUnarchive.Mutation>
      ...{(mutation, _) =>
        <div className=Shared.Item.item>
          <div
            className={Shared.Item.itemButton(DANGER)}
            onClick={e => {
              MeasurableUnarchive.mutate(mutation, measurable.id);
              ReactEvent.Synthetic.stopPropagation(e);
            }}>
            {"Unarchive" |> ste}
          </div>
        </div>
      }
    </MeasurableUnarchive.Mutation>;
};