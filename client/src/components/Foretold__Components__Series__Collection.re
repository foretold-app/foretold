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

  module Query = Foretold__GraphQL.Queries.SeriesCollection;

  type t = Foretold__GraphQL.Queries.SeriesCollection.series;
  type ts = Js.Array.t(Foretold__GraphQL.Queries.SeriesCollection.series);

  let make = (channelId, list: ts) =>
    <div className=Styles.items>
      {
        list
        |> Array.map((x: t) =>
             <div
               className=Styles.itemPositioning
               onClick={
                 _e => Context.Routing.Url.push(SeriesShow(channelId, x.id))
               }>
               <Foretold__Components__Series.Card series=x />
             </div>
           )
        |> ReasonReact.array
      }
    </div>;
};