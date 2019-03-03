open Utils;
open Rationale;
open Queries;
open HandsOnTable;
open MeasurableColumns;
open Style.Grid;

let transformations = [
  link,
  nameAsText,
  measurementCount,
  measurerCount,
  expectedResolutionDate,
  creator,
];

module Styles = {
  open Css;
  let sidebar =
    style([Css.float(`left), left(px(0)), backgroundColor(hex("eee"))]);

  let body = style([marginLeft(px(200)), padding(px(30))]);

  let header =
    style([
      backgroundColor(hex("f5f7f9")),
      padding2(~v=`px(10), ~h=`px(13)),
      border(`px(1), `solid, `hex("e8f2f9")),
      borderRadius(`px(3)),
      marginBottom(`px(10)),
    ]);
};

let component = ReasonReact.statelessComponent("Measurables");
let make = (~channel: string, ~id: string, _children) => {
  ...component,
  render: _self => {
    open Result.Infix;
    let seriesQuery = Queries.GetSeries.Query.make(~id, ());
    let query =
      Queries.GetMeasurables.Query.make(
        ~offset=0,
        ~limit=200,
        ~channel,
        ~seriesId=id,
        (),
      );
    let top =
      Queries.GetSeries.QueryComponent.make(
        ~variables=seriesQuery##variables, series =>
        series.result
        |> ApolloUtils.apolloResponseToResult
        <$> (d => d##series)
        <$> (
          r =>
            switch (r) {
            | Some(k) =>
              <Div styles=[Style.Grid.Styles.flexColumn, Styles.header]>
                <Div styles=[Style.Grid.Styles.flex(1)]>
                  <Div styles=[Style.Grid.Styles.flexRow]>
                    <Div styles=[Style.Grid.Styles.flex(6)]>
                      <h2>
                        <Icon.Icon icon="LAYERS" />
                        {k.name |> Option.default("") |> ste}
                      </h2>
                      <p> {k.description |> Option.default("") |> ste} </p>
                    </Div>
                    <Div styles=[Style.Grid.Styles.flex(1)] />
                  </Div>
                </Div>
                <Div styles=[Style.Grid.Styles.flex(1)] />
              </Div>
            | None => <div />
            }
        )
        |> Result.result(idd, idd)
      );
    let bottom =
      Queries.GetMeasurables.QueryComponent.make(
        ~variables=query##variables, o =>
        o.result
        |> ApolloUtils.apolloResponseToResult
        <$> (d => d##measurables)
        <$> Extensions.Array.concatSomes
        <$> (d => d |> Array.map(Queries.GetMeasurables.toMeasurable))
        <$> (
          measurables =>
            <MeasurableIndex__Table measurables showExtraData=false />
        )
        |> Result.result(idd, idd)
      );
    <div> {top |> ReasonReact.element} {bottom |> ReasonReact.element} </div>
    |> FillWithSidebar.make(~channel=Some(channel))
    |> ReasonReact.element;
  },
};