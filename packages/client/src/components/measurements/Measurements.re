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
        ~head,
        ~block,
        ~measurable: Types.measurable,
        ~reducerParams: Reducer.Types.reducerParams,
      ) => {
    <SLayout
      head={head(
        ~channelId=None,
        ~paginationPage=Reducer.Components.paginationPage(reducerParams),
        (),
      )}
      container=`none>
      {switch (reducerParams.response) {
       | Success(connection) =>
         let measurementsList = connection.edges |> Array.to_list;
         let measurableValueType = measurable.valueType;

         switch (measurable.state) {
         | Some(`JUDGED)
         | Some(`CLOSED_AS_UNRESOLVED) =>
           <MeasurementsTable
             measurementsList
             measurableValueType
             colums=`extended
             block
           />
         | _ =>
           <MeasurementsTable measurementsList measurableValueType block />
         };
       | _ => <Spin />
       }}
    </SLayout>;
  };
};

[@react.component]
let make = (~measurableId, ~head, ~block=`none) => {
  MeasurableGet.component(~id=measurableId)
  |> E.F.apply((measurable: Types.measurable) =>
       <Reducer
         callFnParams={measurable.id}
         subComponent={reducerParams =>
           <Body reducerParams measurable head block />
         }
       />
     );
};