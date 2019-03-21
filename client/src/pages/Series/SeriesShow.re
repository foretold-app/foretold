open Utils;
open Rationale;
open Style.Grid;
open Foretold__GraphQL;

module Styles = {
  open Css;
  let header =
    style([
      backgroundColor(hex("f5f7f9")),
      padding2(~v=`px(10), ~h=`px(13)),
      border(`px(1), `solid, `hex("e8f2f9")),
      borderRadius(`px(3)),
      marginBottom(`px(10)),
    ]);

  let topPart =
    style([
      maxHeight(`px(300)),
      overflowY(`auto),
      borderRadius(`px(2)),
      border(`px(1), `solid, `hex("ddd")),
      marginTop(`em(2.)),
      marginBottom(`em(2.)),
    ]);
};

type state = {selected: option(string)};

type action =
  | UpdateSelected(string);

let component = ReasonReact.reducerComponent("Measurables");

let seriesHero = (series: DataModel.Series.t) =>
  <Div flexDirection=`column styles=[Styles.header]>
    <Div flex=1>
      <Div flexDirection=`row>
        <Div flex=6>
          {
            series.name
            |> E.O.React.fmapOrNull(name =>
                 <h2> <Icon.Icon icon="LAYERS" /> {name |> ste} </h2>
               )
          }
          {
            series.description
            |> E.O.React.fmapOrNull(description =>
                 description |> ste |> E.React.inP
               )
          }
          {
            switch (series.creator) {
            | Some({name: Some(name), id}) =>
              <div className=C.Shared.Item.item>
                <a href={DataModel.Url.toString(AgentShow(id))}>
                  {name |> ste}
                </a>
              </div>
            | _ => E.React.null
            }
          }
        </Div>
        <Div flex=1 />
      </Div>
    </Div>
    <Div flex=1 />
  </Div>;

let make =
    (~channel: string, ~id: string, ~loggedInUser: Queries.User.t, _children) => {
  ...component,
  initialState: () => {selected: None},
  reducer: (action, _state) =>
    switch (action) {
    | UpdateSelected(str) => ReasonReact.Update({selected: Some(str)})
    },
  render: ({state, send}) => {
    let medium =
      Queries.Measurables.componentWithSeries(channel, id, measurables =>
        <C.Measurables.SeriesTable
          measurables
          selected={state.selected}
          onClick={id => send(UpdateSelected(id))}
        />
      );

    Queries.Series.component(~id)
    |> E.F.apply(series =>
         <>
           <SLayout.Header>
             {
               SLayout.seriesHead(
                 channel,
                 series
                 |> E.O.bind(_, (s: DataModel.Series.t) => s.name)
                 |> E.O.default(""),
               )
             }
           </SLayout.Header>
           <SLayout.MainSection>
             {series |> E.O.React.fmapOrNull(seriesHero)}
             <div className=Styles.topPart> medium </div>
             {
               state.selected
               |> E.O.React.fmapOrNull(elId =>
                    <C.Measurable.FullPresentation id=elId loggedInUser />
                  )
             }
           </SLayout.MainSection>
         </>
       );
  },
};