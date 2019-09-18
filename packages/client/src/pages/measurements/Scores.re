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

module Pagination = {
  let component = ReasonReact.statelessComponent("Pagination");
  let make = (~reducerParams: Reducer.Types.reducerParams, _children) => {
    ...component,
    render: _ =>
      <>
        <Div>
          <Div
            styles=[
              Css.style([
                FC.Base.BaseStyles.floatLeft,
                Css.paddingTop(`em(0.2)),
              ]),
            ]>
            <FC.Tab isActive=true>
              {"Predictions" |> ReasonReact.string}
            </FC.Tab>
            <FC.Tab isActive=false> {"Scores" |> ReasonReact.string} </FC.Tab>
          </Div>
        </Div>
        <Div>
          <Div
            float=`right
            styles=[Css.style([FC.PageCard.HeaderRow.Styles.itemTopPadding])]>
            {Reducer.Components.paginationPage(reducerParams)}
          </Div>
        </Div>
      </>,
  };
};

module Body = {
  let component = ReasonReact.statelessComponent("Body");
  let make =
      (
        ~reducerParams: Reducer.Types.reducerParams,
        ~loggedInUser,
        ~measurable: Types.measurable,
        _children,
      ) => {
    ...component,
    render: _ =>
      <SLayout head={<Pagination reducerParams />}>
        {switch (reducerParams.response) {
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
           | _ => MeasurementsTable.make(~loggedInUser, ~measurementsList, ())
           };
         | _ => <SLayout.Spin />
         }}
      </SLayout>,
  };
};

let component = ReasonReact.statelessComponent("Scores");
let make = (~measurableId: string, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _ => {
    MeasurableGet2.component(~id=measurableId)
    |> E.F.apply((measurable: Types.measurable) =>
         <Reducer
           callFnParams={measurable.id}
           subComponent={reducerParams =>
             <Body reducerParams measurable loggedInUser />
           }
         />
       );
  },
};