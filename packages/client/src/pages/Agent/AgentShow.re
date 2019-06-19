module GetMeasurablesReducerConfig = {
  type itemType = Context.Primary.Measurement.t;
  type callFnParams = string;

  let getId = (e: Context.Primary.Measurement.t) => e.id;
  let callFn = (e: callFnParams) =>
    Foretold__GraphQL.Queries.AgentMeasurements.componentWithMeasurementConnection(
      ~id=e,
    );

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module SelectWithPaginationReducer =
  SelectWithPaginationReducerFunctor.Make(GetMeasurablesReducerConfig);

let component = ReasonReact.statelessComponent("AgentMeasurables2");
type pageParams = {id: string};

let make =
    (
      ~pageParams: pageParams,
      ~loggedInUser: Context.Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ => {
    SelectWithPaginationReducer.make(
      ~itemsPerPage=20,
      ~callFnParams=pageParams.id,
      ~subComponent=selectWithPaginationParams =>
      SLayout.LayoutConfig.make(
        ~head=
          switch (selectWithPaginationParams.selection) {
          | Some(_selection) =>
            <>
              {SelectWithPaginationReducer.Components.deselectButton(
                 selectWithPaginationParams.send,
               )}
              {SelectWithPaginationReducer.Components.correctButtonDuo(
                 selectWithPaginationParams,
               )}
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
            let onSelectClb = (e: Client.Context.Primary.Measurement.t) => {
              SelectWithPaginationReducer.Components.sendSelectItem(
                selectWithPaginationParams,
                e.id,
              );
            };
            let measurementsList = connection.edges |> Array.to_list;
            C.Measurements.Table.make2(
              ~measurementsList,
              ~onSelect=onSelectClb,
              (),
            );
          | _ => <div />
          },
      )
      |> layout
    )
    |> E.React.makeToEl;
  },
};