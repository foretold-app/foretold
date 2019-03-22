open Utils;
open Style.Grid;
open Foretold__GraphQL;

module Card = {
  let component = ReasonReact.statelessComponent("SeriesCard");

  module Styles = {
    open Css;
    let card =
      style([
        backgroundColor(hex("eee")),
        padding2(~v=`em(0.2), ~h=`em(0.7)),
        borderRadius(`px(4)),
        backgroundColor(`hex("ebe9f3")),
        border(`px(1), `solid, `hex("dadae4")),
        color(`hex("0b2d67")),
        cursor(`pointer),
        minWidth(`em(23.)),
        selector(":hover", [backgroundColor(`hex("dad7e8"))]),
        selector("p", [marginBottom(`px(0))]),
      ]);

    let heading =
      style([
        color(`hex("0b2d67")),
        fontWeight(`bold),
        fontSize(`em(1.2)),
      ]);
  };

  let make = (~series: Queries.SeriesCollection.series, _children) => {
    ...component,
    render: _self =>
      <div
        className=Styles.card
        onClick={_e => DataModel.Url.push(SeriesShow("general", series.id))}>
        <span className=Styles.heading>
          <Icon.Icon icon="LAYERS" />
          {series.name |> E.O.default("") |> ste}
        </span>
        {
          series.description
          |> E.O.fmap(d => d |> ste |> E.React.inP)
          |> E.O.React.defaultNull
        }
        {
          series.measurableCount
          |> E.O.fmap(d =>
               d |> string_of_int |> (e => e ++ " items") |> ste |> E.React.inP
             )
          |> E.O.React.defaultNull
        }
      </div>,
  };
};