module ReducerConfig = {
  type itemType = Types.measurable;
  type callFnParams = string;

  let getId = (e: itemType) => e.id;

  let onItemDeselected = _ => ();
  let onItemSelected = (_: option(itemType)) => ();

  let callFn = (creatorId: callFnParams) =>
    MeasurablesGet.component(
      ~creatorId=Some(creatorId),
      ~states=Some([|`OPEN|]),
      (),
    );

  let isEqual = (a: itemType, b: itemType) => a.id == b.id;
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

[@react.component]
let make = (~pageParams: Types.pageParams) => {
  <Reducer
    callFnParams={pageParams.id}
    subComponent={(selectWithPaginationParams: Reducer.reducerParams) =>
      <SLayout
        head={
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
          }
        }>
        {switch (
           selectWithPaginationParams.response,
           selectWithPaginationParams.selection,
         ) {
         | (_, Some(measurable)) => <MeasurablePage measurable />

         | (Success(connection), None) =>
           <MeasurableIndexTable
             measurables={connection.edges}
             showExtraData=true
             onSelect={(e: Types.measurable) =>
               Reducer.Components.sendSelectItem(
                 selectWithPaginationParams,
                 e.id,
               )
             }
           />

         | _ => <div />
         }}
      </SLayout>
    }
  />;
};