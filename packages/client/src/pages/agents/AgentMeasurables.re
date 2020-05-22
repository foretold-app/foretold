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
    subComponent={(reducerState: Reducer.state) =>
      <SLayout
        head={
          switch (reducerState.selection) {
          | Some(_selection) =>
            <>
              <Reducer.Components.DeselectButton send={reducerState.send} />
              {Reducer.Components.correctButtonDuo(reducerState)}
            </>
          | None => <> {Reducer.Components.correctButtonDuo(reducerState)} </>
          }
        }>
        {switch (reducerState.response, reducerState.selection) {
         | (_, Some(measurable)) => <MeasurablePage measurable />

         | (Success(connection), None) =>
           <MeasurableIndexTable
             measurables={connection.edges}
             showExtraData=true
             onSelect={(e: Types.measurable) =>
               Reducer.Components.sendSelectItem(reducerState, e.id)
             }
           />

         | _ => <div />
         }}
      </SLayout>
    }
  />;
};