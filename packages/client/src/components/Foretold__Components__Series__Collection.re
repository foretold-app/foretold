module SeriesCards = {
  module Styles = {
    open Css;
    let items =
      style([
        display(`flex),
        flexWrap(`wrap),
        marginTop(`em(1.0)),
        marginBottom(`em(1.0)),
      ]);

    let itemPositioning =
      style([
        Css.float(`left),
        margin4(
          ~top=`em(0.0),
          ~left=`em(0.0),
          ~right=`em(0.5),
          ~bottom=`em(0.5),
        ),
      ]);
  };

  type t = Foretold__GraphQL.Queries.SeriesCollection.series;
  type ts = Js.Array.t(t);

  let make = (channelId, list: ts) =>
    <div className=Styles.items>
      {list
       |> E.A.fmapi((i, x: t) =>
            <div
              key={i |> string_of_int}
              className=Styles.itemPositioning
              onClick={_e => Routing.Url.push(SeriesShow(channelId, x.id))}>
              <Foretold__Components__Series.Card series=x />
            </div>
          )
       |> ReasonReact.array}
    </div>;
};