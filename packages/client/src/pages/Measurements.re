open Style.Grid;

module ReducerConfig = {
  type itemType = Types.measurement;
  type callFnParams = string;

  let getId = (e: itemType) => e.id;
  let callFn = (measurableId: callFnParams) =>
    MeasurementsGet.component(~measurableId=Some(measurableId), ());

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

let component = ReasonReact.statelessComponent("Measurements");
let make = (~measurableId: string, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _ => {
    let pagination = (reducerParams: Reducer.Types.reducerParams) =>
      <Div>
        <Div
          float=`right
          styles=[
            Css.style([
              FC.PageCard.HeaderRow.Styles.itemTopPadding,
              FC.PageCard.HeaderRow.Styles.itemBottomPadding,
            ]),
          ]>
          {Reducer.Components.paginationPage(reducerParams)}
        </Div>
      </Div>;

    MeasurableGet2.component(~id=measurableId)
    |> E.F.apply((measurable: Types.measurable) =>
         Reducer.make(
           ~itemsPerPage=20,
           ~callFnParams=measurable.id,
           ~subComponent=selectWithPaginationParams =>
           SLayout.LayoutConfig.make(
             ~head=pagination(selectWithPaginationParams),
             ~body=
               switch (selectWithPaginationParams.response) {
               | Success(connection) =>
                 let measurementsList = connection.edges |> Array.to_list;

                 switch (measurable.state) {
                 | Some(`JUDGED)
                 | Some(`CLOSED_AS_UNRESOLVED) =>
                   MeasurementsTable.makeExtended(
                     ~measurementsList,
                     ~loggedInUser,
                     (),
                   )
                 | _ =>
                   MeasurementsTable.make(
                     ~loggedInUser,
                     ~measurementsList,
                     (),
                   )
                 };
               | _ => <SLayout.Spin />
               },
           )
           |> SLayout.FullPage.makeWithEl
         )
         |> E.React.makeToEl
       );
  },
};