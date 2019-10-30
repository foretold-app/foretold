[@bs.config {jsx: 3}];

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

module Body = {
  [@react.component]
  let make =
      (
        ~reducerParams: Reducer.Types.reducerParams,
        ~measurable: Types.measurable,
        ~head,
      ) => {
    <SLayout
      head={head(
        ~channelId=Some(""),
        ~paginationPage=Reducer.Components.paginationPage(reducerParams),
        (),
      )}
      isFluid=true>
      {switch (reducerParams.response) {
       | Success(connection) =>
         let measurementsList = connection.edges |> Array.to_list;
         let measurableValueType = measurable.valueType;

         switch (measurable.state) {
         | Some(`JUDGED)
         | Some(`CLOSED_AS_UNRESOLVED) =>
           MeasurementsTable.makeExtended(
             ~measurableValueType,
             ~measurementsList,
             (),
           )
         | _ =>
           MeasurementsTable.make(~measurementsList, ~measurableValueType, ())
         };
       | _ => <Spin />
       }}
    </SLayout>;
  };
};

[@react.component]
let make = (~measurableId: string, ~head) => {
  MeasurableGet.component(~id=measurableId)
  |> E.F.apply((measurable: Types.measurable) =>
       <Reducer
         callFnParams={measurable.id}
         subComponent={reducerParams => <Body reducerParams measurable head />}
       />
     );
};
