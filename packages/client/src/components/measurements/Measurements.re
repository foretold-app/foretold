module ReducerConfig = {
  type itemType = Types.measurement;
  type callFnParams = string;

  let getId = (e: itemType) => e.id;

  let onItemDeselected = (_, _) => ();
  let onItemSelected = (_, _) => ();

  let callFn = (measurableId: callFnParams) =>
    MeasurementsGet.component(~measurableId=Some(measurableId), ());

  let isEqual = (a: itemType, b: itemType) => {
    let dates =
      switch (a.updatedAt, b.updatedAt) {
      | (Some(a), Some(b)) => MomentRe.Moment.isSame(a, b)
      | _ => true
      };
    let votes =
      switch (a.totalVoteAmount, b.totalVoteAmount) {
      | (Some(a), Some(b)) => a === b
      | (None, Some(_)) => false
      | (Some(_), None) => false
      | _ => true
      };
    dates && votes;
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
        ~reducerParams: Reducer.state,
      ) => {
    <SLayout
      head={head(
        ~channelId=None,
        ~paginationPage=
          <Reducer.Components.PaginationPage state=reducerParams />,
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