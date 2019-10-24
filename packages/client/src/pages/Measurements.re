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
  let component = ReasonReact.statelessComponent("Body");
  let make =
      (
        ~reducerParams: Reducer.Types.reducerParams,
        ~measurable: Types.measurable,
        ~head,
        _children,
      ) => {
    ...component,
    render: _ =>
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
             MeasurementsTable.make(
               ~measurementsList,
               ~measurableValueType,
               (),
             )
           };
         | _ => <Spin />
         }}
      </SLayout>,
  };
};

let component = ReasonReact.statelessComponent("Measurements");
let make = (~measurableId: string, ~head, _children) => {
  ...component,
  render: _ => {
    MeasurableGet.component(~id=measurableId)
    |> E.F.apply((measurable: Types.measurable) =>
         <Reducer
           callFnParams={measurable.id}
           subComponent={reducerParams =>
             <Body reducerParams measurable head />
           }
         />
       );
  },
};
