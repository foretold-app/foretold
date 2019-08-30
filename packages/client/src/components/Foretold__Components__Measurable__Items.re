open Utils;
open ReactMarkdown;
open Style.Grid;
module Shared = Foretold__Components__Shared;

/**
 * Using <Components /> style in React.js is preferable then
 * functions. Try first of all to create new component.
 **/

let formatDate = e =>
  e |> E.O.fmap(E.M.format(E.M.format_simple)) |> E.O.default("");

let dateOnStyle =
  Css.(
    style([
      marginLeft(`em(0.2)),
      lineHeight(`em(1.56)),
      fontSize(`em(1.1)),
    ])
  );

let dateItem = (~m: Types.measurable, ~showOn=true, ~onStyle=dateOnStyle, ()) =>
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

let link = (~m: Types.measurable) => {
  open Css;
  let name = style([fontSize(`em(1.)), color(`hex("333"))]);

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
        <span className=name> {m.name |> ste} </span>
        {dateItem(~m, ()) |> E.O.React.defaultNull}
      </>;
    }}
  </Providers.AppContext.Consumer>;
};

let description = (~m: Types.measurable) =>
  switch (m.labelCustom) {
  | Some("")
  | None => None
  | Some(text) => Some(<Markdown source=text />)
  };

let endpointResponse = (~m: Types.measurable) =>
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

let questionLink = (~m: Types.measurable) =>
  <Link.Jsx2
    className=Shared.Item.item
    linkType={Internal(MeasurableShow(m.channelId, m.id))}>
    {"Link to This Question" |> Utils.ste}
  </Link.Jsx2>;

let creatorLink = (~m: Types.measurable) =>
  m.creator
  |> E.O.fmap((c: Types.agent) =>
       <Link.Jsx2
         linkType={
           Internal(Agent({agentId: c.id, subPage: AgentMeasurements}))
         }
         className=Shared.Item.item>
         {c.name |> E.O.default("") |> ste}
       </Link.Jsx2>
     );

let editLink = (~m: Types.measurable) =>
  <div className=Shared.Item.item>
    <Link.Jsx2
      linkType={Internal(MeasurableEdit(m.id))}
      className={Shared.Item.itemButton(NORMAL)}>
      {"Edit" |> ste}
    </Link.Jsx2>
  </div>;

let channelLink = (~m: Types.measurable) =>
  <div className=Shared.Item.item>
    <Link.Jsx2
      linkType={Internal(ChannelShow(m.channelId))}
      className=Shared.Item.item>
      {switch (m.channel) {
       | Some(channel) => "#" ++ channel.name |> ste
       | _ => "#channel" |> ste
       }}
    </Link.Jsx2>
  </div>;

let measurements = (~m: Types.measurable) =>
  switch (m.measurementCount) {
  | Some(0) => None
  | None => None
  | Some(count) =>
    let popoverContent =
      "Number of Predictions: " ++ string_of_int(count) |> ste;
    Some(
      <AntdPopover content=popoverContent trigger=`hover placement=`top>
        <div className=Shared.Item.item>
          <Icon.Icon icon="BULB" />
          {count |> string_of_int |> ste}
        </div>
      </AntdPopover>,
    );
  };

let measurers = (~m: Types.measurable) =>
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

let id = (~m: Types.measurable, ()) =>
  <div className=Shared.Item.id>
    {"ID:  " ++ m.id |> ReasonReact.string}
  </div>;

let series = (~m: Types.measurable, ~channelId=None, ()) => {
  m.series
  |> E.O.bind(_, r =>
       switch (r.name) {
       | Some(name) =>
         Some(
           <div className=Shared.Item.item>
             <Link.Jsx2 linkType={Internal(SeriesShow(m.channelId, r.id))}>
               <Icon.Icon icon="LAYERS" />
               {name |> ste}
             </Link.Jsx2>
           </div>,
         )
       | None => None
       }
     );
};

let expectedResolutionDate = (~m: Types.measurable) =>
  <div className=Shared.Item.item>
    {"Resolves on " ++ (m.expectedResolutionDate |> formatDate) |> ste}
  </div>;

let resolutionEndpoint = (~m: Types.measurable) =>
  switch (m.resolutionEndpoint |> E.O.default("")) {
  | "" => None
  | text =>
    Some(
      <div className=Shared.Item.item> {"Endpoint: " ++ text |> ste} </div>,
    )
  };

let archiveButton = (~m: Types.measurable) =>
  MeasurableArchive.Mutation.make((mutation, _) =>
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
  )
  |> E.React.el;

let unArchiveButton = (~m: Types.measurable) =>
  MeasurableUnarchive.Mutation.make((mutation, _) =>
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
  )
  |> E.React.el;

let archiveOption = (~m: Types.measurable) =>
  m.isArchived == Some(true) ? unArchiveButton(~m) : archiveButton(~m);

module FloatPoint = {
  let component = ReasonReact.statelessComponent("FloatPoint");
  let make = (~value: float, _children) => {
    ...component,
    render: _self => {
      <Div flexDirection=`column>
        <Div flex={`num(1.)}> {value |> E.Float.toFixed |> Utils.ste} </Div>
      </Div>;
    },
  };
};

module FloatCdf = {
  let component = ReasonReact.statelessComponent("FloatCdf");
  let make =
      (
        ~value: MeasurementValue.FloatCdf.t,
        ~competitorType: Types.competitorType,
        ~valueText: option(string)=None,
        ~width=175,
        ~height=30,
        _children,
      ) => {
    ...component,
    render: _self => {
      let color =
        competitorType == `AGGREGATION ? `hex("b1b9c6") : `hex("487192");

      let dist =
        value
        |> MeasurementValue.toPdf
        |> MeasurementValue.FloatCdf.toJs
        |> (data => <SmallCdfChart data width height color />);

      <Div flexDirection=`row>
        <Div flex={`num(1.)}>
          <Div flexDirection=`column>
            <Div flex={`num(1.)}> {"25" |> Utils.ste} </Div>
            <Div flex={`num(1.)}>
              {valueText |> E.O.default("") |> Utils.ste}
            </Div>
          </Div>
        </Div>
        <Div flex={`num(1.)}> dist </Div>
      </Div>;
    },
  };
};

module Binary = {
  let component = ReasonReact.statelessComponent("Binary");
  let make = (~value: bool, _children) => {
    ...component,
    render: _self => {
      <Div flexDirection=`column>
        <Div flex={`num(1.)}> {(value ? "True" : "False") |> Utils.ste} </Div>
      </Div>;
    },
  };
};

module UnresolvableResolution = {
  let component = ReasonReact.statelessComponent("UnresolvableResolution");
  let make = _children => {
    ...component,
    render: _self => {
      <Div flexDirection=`column>
        <Div flex={`num(1.)}> {"Closed Without Answer" |> Utils.ste} </Div>
      </Div>;
    },
  };
};

module Percentage = {
  let component = ReasonReact.statelessComponent("Percentage");
  let make = (~value: float, _children) => {
    ...component,
    render: _self => {
      <Div flexDirection=`column>
        <Div flex={`num(1.)}>
          <FC__PercentageShower precision=8 percentage=value />
        </Div>
        <Div flex={`num(1.)}> {"Median" |> Utils.ste} </Div>
      </Div>;
    },
  };
};

module AggregationResolution = {
  let component = ReasonReact.statelessComponent("AggregationResolution");
  let make = (~measurable: Types.measurable, _children) => {
    ...component,
    render: _self => {
      switch (measurable.previousAggregate, measurable.outcome) {
      | (_, Some(measurement)) =>
        switch (measurement.value) {
        | Ok(`FloatPoint(r)) => <FloatPoint value=r />
        | Ok(`FloatCdf(r)) =>
          <FloatCdf
            value=r
            valueText={measurement.valueText}
            competitorType={measurement.competitorType}
          />
        | Ok(`Binary(r)) => <Binary value=r />
        | Ok(`UnresolvableResolution(r)) => <UnresolvableResolution />
        | _ => ReasonReact.null
        }

      | (Some(measurement), None) =>
        switch (measurement.value) {
        | Ok(`FloatCdf(r)) =>
          <FloatCdf
            value=r
            valueText={measurement.valueText}
            competitorType={measurement.competitorType}
          />
        | Ok(`Percentage(r)) => <Percentage value=r />
        | _ => ReasonReact.null
        }

      | _ => ReasonReact.null
      };
    },
  };
};