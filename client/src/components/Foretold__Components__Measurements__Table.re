open Utils;
open MomentRe;
open Style.Grid;
open Css;

type measurement = DataModel.Measurement.t;

let isJudgement = (m: measurement) => m.competitorType == `OBJECTIVE;

module Styles = {
  open Css;

  let middle =
    style([
      textAlign(`center),
      fontSize(`em(1.4)),
      fontWeight(`num(800)),
      color(`hex("7d7ea2")),
      marginTop(`px(6)),
    ]);

  let percentiles =
    style([
      top(`em(0.2)),
      right(`em(0.5)),
      fontSize(`em(1.1)),
      position(`absolute),
      marginRight(`px(6)),
      color(`hex("5d7682")),
      display(`none),
    ]);

  let group =
    style([
      borderRadius(`px(2)),
      border(`px(1), `solid, `hex("e1eaf1")),
    ]);

  let row =
    style([
      width(`percent(100.0)),
      selector(" h2", [marginTop(px(2))]),
      display(`flex),
      flexDirection(`row),
      backgroundColor(`hex("eaf0f5")),
      selector(":last-child", [borderBottom(`px(0), `solid, hex("fff"))]),
      selector(":hover", [selector(" .foo", [display(`inline)])]),
    ]);

  let axisRow =
    style([
      width(`percent(100.0)),
      selector(" h2", [marginTop(px(2))]),
      display(`flex),
      flexDirection(`row),
      backgroundColor(`hex("e2eaef")),
      selector(":last-child", [borderBottom(`px(0), `solid, hex("fff"))]),
    ]);

  let axisRightColumn = style([flex(3), display(`flex)]);

  let mainColumn =
    style([
      flex(1),
      display(`flex),
      flexDirection(`column),
      position(`relative),
    ]);

  let item =
    style([
      float(`left),
      fontSize(`px(14)),
      marginRight(`px(18)),
      color(`hex("red")),
    ]);

  let rightColumn =
    style([
      flex(3),
      display(`flex),
      backgroundColor(`hex("f3f5f7")),
      borderBottom(`px(1), `solid, `hex("e8eef3")),
      borderTopRightRadius(`px(2)),
      borderBottomRightRadius(`px(2)),
    ]);

  let rightColumnInner =
    style([flex(1), padding2(~v=`px(4), ~h=`px(10))]);

  let mainColumnTop = style([flex(1), paddingLeft(px(2))]);

  let descriptionStyle =
    style([
      marginTop(`px(5)),
      selector(
        " p",
        [color(`rgba((80, 80, 80, 0.78))), marginBottom(`px(0))],
      ),
    ]);

  let date =
    style([
      float(`right),
      color(`rgba((35, 35, 35, 0.34))),
      marginTop(`px(1)),
    ]);

  let agentStyle =
    style([color(`rgb((102, 121, 134))), fontSize(`em(1.1))]);
};

module Helpers = {
  let stringOfFloat = Js.Float.toPrecisionWithPrecision(_, ~digits=3);
  let smallDistribution = (m: measurement, g: (float, float)) =>
    switch (m.value) {
    | Belt.Result.Ok(`FloatCdf(r)) =>
      r
      |> Value.toPdf(~bucketSize=20)
      |> Value.FloatCdf.toPoints
      |> (data => <WideChart data bounds=g />)
    | Belt.Result.Ok(`FloatPoint(r)) =>
      <div className=Styles.middle> {stringOfFloat(r) |> ste} </div>
    | _ => "" |> ste
    };

  let formatDate = e =>
    e |> E.O.fmap(E.M.format(E.M.format_simple)) |> E.O.default("");

  let description = (~m: measurement) =>
    switch (m.description |> E.O.default("")) {
    | "" => E.React.null
    | text =>
      <div className=Styles.descriptionStyle> <p> {text |> ste} </p> </div>
    };

  let relevantAt = (~m: measurement) =>
    <div className=Styles.date> {m.relevantAt |> formatDate |> ste} </div>;

  let stringOfFloat = Js.Float.toPrecisionWithPrecision(_, ~digits=3);

  let getFloatCdf = (e: Belt.Result.t(Value.t, string)) =>
    switch (e) {
    | Belt.Result.Ok(`FloatCdf(r)) => Some(r)
    | _ => None
    };

  let bounds = (m: Js_array.t(measurement)) => {
    let itemBounds =
      m
      |> E.A.keepMap(_, r => getFloatCdf(r.value))
      |> E.A.fmap(r =>
           (
             E.FloatCdf.firstAboveValue(0.05, r),
             E.FloatCdf.firstAboveValue(0.95, r),
           )
         );
    let min =
      itemBounds
      |> E.A.keepMap(_, ((min, _)) => min)
      |> E.A.fold_left((a, b) => a < b ? a : b, max_float);
    let max =
      itemBounds
      |> E.A.keepMap(_, ((_, max)) => max)
      |> E.A.fold_left((a, b) => a > b ? a : b, min_float);
    (min, max);
  };

  let floatCdf = e =>
    switch (e##value) {
    | Belt.Result.Ok(`FloatCdf(r)) => Some(r)
    | _ => None
    };

  let percentile = (n, e) =>
    Rationale.Option.Infix.(
      floatCdf(e)
      >>= E.FloatCdf.firstAboveValue(n)
      <$> stringOfFloat
      |> E.O.default("")
    );

  let toChartMeasurement = (m: measurement) =>
    switch (m.value) {
    | Belt.Result.Ok(`FloatCdf(r)) =>
      switch (
        E.FloatCdf.firstAboveValue(0.05, r),
        E.FloatCdf.firstAboveValue(0.95, r),
      ) {
      | (Some(low), Some(high)) => Some((low, high))
      | _ => None
      }
    | _ => None
    };

  let toPercentiles = (m: measurement) =>
    switch (m.value) {
    | Belt.Result.Ok(`FloatCdf(r)) =>
      switch (
        E.FloatCdf.firstAboveValue(0.05, r),
        E.FloatCdf.firstAboveValue(0.95, r),
      ) {
      | (Some(low), Some(high)) =>
        let foo = stringOfFloat(low) ++ " to " ++ stringOfFloat(high);
        Some(foo);
      | _ => None
      }
    | _ => None
    };

  let measurerLink = (~m: measurement) => {
    let agent = m.agent;
    let aLink =
      switch (agent, agent |> E.O.bind(_, DataModel.Agent.name)) {
      | (Some(agent), Some(name)) =>
        <a
          href={DataModel.Url.toString(AgentShow(agent.id))}
          className=Styles.agentStyle>
          {name |> ste}
        </a>
      | _ => E.React.null
      };

    let judgementStyle =
      style([
        selector(
          " h3",
          [
            color(`rgba((55, 47, 68, 0.85))),
            marginBottom(`px(0)),
            fontSize(`em(1.15)),
            fontWeight(`num(800)),
          ],
        ),
        selector(" a", [fontSize(`em(0.9))]),
      ]);
    let isJudge = isJudgement(m);

    if (isJudge) {
      <div className=judgementStyle>
        <h3> {"Judgement" |> ste} </h3>
        {aLink !== ("" |> ste) ? <span> {"by " |> ste} </span> : <span />}
        aLink
      </div>;
    } else {
      aLink;
    };
  };
};

let stringOfFloat = Js.Float.toPrecisionWithPrecision(_, ~digits=3);

let make = (ms: list(measurement)) => {
  let _bounds = Helpers.bounds(ms |> E.A.of_list);
  let items =
    ms
    |> E.L.sort((a: measurement, b: measurement) =>
         switch (a.createdAt, b.createdAt) {
         | (Some(c), Some(d)) =>
           Moment.toUnix(c) < Moment.toUnix(d) ? 1 : (-1)
         | _ => 0
         }
       )
    |> E.L.fmap((m: measurement) =>
         <div className=Styles.row key={m.id}>
           <div className=Styles.mainColumn>
             <div className=Styles.mainColumnTop>
               {Helpers.smallDistribution(m, _bounds)}
             </div>
             {
               switch (Helpers.toPercentiles(m)) {
               | Some(a) =>
                 <div className={Styles.percentiles ++ " " ++ "foo"}>
                   {a |> ste}
                 </div>
               | _ => <span />
               }
             }
           </div>
           <div className=Styles.rightColumn>
             <div className=Styles.rightColumnInner>
               <Div flexDirection=`column>
                 <Div flex=1>
                   <Div flexDirection=`row>
                     <Div flex=4> {Helpers.measurerLink(~m)} </Div>
                     <Div flex=1> {Helpers.relevantAt(~m)} </Div>
                   </Div>
                 </Div>
                 <Div flex=1> {Helpers.description(~m)} </Div>
               </Div>
             </div>
           </div>
         </div>
       )
    |> E.A.of_list
    |> ReasonReact.array;
  E.React.showIf(
    ms |> E.L.length > 0,
    <div className=Styles.group>
      items
      <div className=Styles.axisRow>
        <div className=Styles.mainColumn>
          <div className=Styles.mainColumnTop> <XAxis bounds=_bounds /> </div>
        </div>
        <div className=Styles.axisRightColumn />
      </div>
    </div>,
  );
};