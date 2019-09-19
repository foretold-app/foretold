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
        ~loggedInUser,
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
        )}>
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
         | _ => <Spin />
         }}
      </SLayout>,
  };
};

let component = ReasonReact.statelessComponent("Measurements");
let make =
    (~measurableId: string, ~loggedInUser: Types.user, ~head, _children) => {
  ...component,
  render: _ => {
    MeasurableGet2.component(~id=measurableId)
    |> E.F.apply((measurable: Types.measurable) =>
         <Reducer
           callFnParams={measurable.id}
           subComponent={reducerParams =>
             <Body reducerParams measurable loggedInUser head />
           }
         />
       );
  },
};