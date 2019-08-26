module ReducerConfig = {
  type itemType = Types.measurement;
  type callFnParams = string;

  let getId = (e: Types.measurement) => e.id;
  let callFn = (agentId: callFnParams) =>
    MeasurementsGet.component(~agentId=Some(agentId), ());

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationReducerFunctor.Make(ReducerConfig);

let component = ReasonReact.statelessComponent("AgentMeasurements");
type pageParams = {id: string};

let make =
    (
      ~pageParams: pageParams,
      ~loggedInUser: Types.user,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ => {
    Reducer.make(
      ~itemsPerPage=20,
      ~callFnParams=pageParams.id,
      ~subComponent=selectWithPaginationParams =>
      SLayout.LayoutConfig.make(
        ~head=
          switch (selectWithPaginationParams.selection) {
          | Some(_selection) =>
            <>
              {Reducer.Components.deselectButton(
                 selectWithPaginationParams.send,
               )}
              {Reducer.Components.correctButtonDuo(selectWithPaginationParams)}
            </>
          | None => <div />
          },
        ~body=
          switch (
            selectWithPaginationParams.response,
            selectWithPaginationParams.selection,
          ) {
          | (_, Some(measurement)) =>
            switch (measurement.measurable) {
            | Some(measurable) =>
              <C.Measurable.FullPresentation id={measurable.id} loggedInUser />
            | _ => <div />
            }

          | (Success(connection), None) =>
            let onSelectClb = (e: Types.measurement) => {
              Reducer.Components.sendSelectItem(
                selectWithPaginationParams,
                e.id,
              );
            };

            let measurementsList = connection.edges |> Array.to_list;

            C.Measurements.Table.makeAgentPredictionsTable(
              ~measurementsList,
              ~onSelect=onSelectClb,
              (),
            );
          | _ => <SLayout.Spin />
          },
      )
      |> layout
    )
    |> E.React.makeToEl;
  },
};