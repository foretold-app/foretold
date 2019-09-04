module ReducerConfig = {
  type itemType = Types.measurable;
  type callFnParams = string;

  let getId = (e: Types.measurable) => e.id;

  let callFn = (creatorId: callFnParams) =>
    MeasurablesGet.component(
      ~creatorId=Some(creatorId),
      ~states=[|Some(`OPEN)|],
      (),
    );

  let isEqual = (a: itemType, b: itemType) => a.id == b.id;
};

module Reducer = PaginationReducerFunctor.Make(ReducerConfig);

let component = ReasonReact.statelessComponent("AgentMeasurables");
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
    let lmake = SLayout.LayoutConfig.make;
    Reducer.make(
      ~itemsPerPage=20,
      ~callFnParams=pageParams.id,
      ~subComponent=selectWithPaginationParams =>
      lmake(
        ~head=
          switch (selectWithPaginationParams.selection) {
          | Some(_selection) =>
            <>
              {Reducer.Components.deselectButton(
                 selectWithPaginationParams.send,
               )}
              {Reducer.Components.correctButtonDuo(selectWithPaginationParams)}
            </>
          | None =>
            <>
              {Reducer.Components.correctButtonDuo(selectWithPaginationParams)}
            </>
          },
        ~body=
          switch (
            selectWithPaginationParams.response,
            selectWithPaginationParams.selection,
          ) {
          | (_, Some(measurable)) =>
            <MeasurableFullPresentation id={measurable.id} loggedInUser />
          | (Success(connection), None) =>
            <MeasurableIndexTable
              measurables={connection.edges}
              showExtraData=true
              onSelect={e =>
                Reducer.Components.sendSelectItem(
                  selectWithPaginationParams,
                  e.id,
                )
              }
            />
          | _ => <div />
          },
      )
      |> layout
    )
    |> E.React.makeToEl;
  },
};