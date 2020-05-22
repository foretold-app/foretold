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
    subComponent={(state: Reducer.state) =>
      <SLayout
        head={
          switch (state.selection) {
          | Some(_selection) =>
            <>
              <Reducer.Components.DeselectButton send={state.send} />
              <Reducer.Components.CorrectButtonDuo state />
            </>
          | None => <Reducer.Components.CorrectButtonDuo state />
          }
        }>
        {switch (state.response, state.selection) {
         | (_, Some(measurable)) => <MeasurablePage measurable />

         | (Success(connection), None) =>
           <MeasurableIndexTable
             measurables={connection.edges}
             showExtraData=true
             onSelect={(e: Types.measurable) =>
               Reducer.Components.sendSelectItem(state, e.id)
             }
           />

         | _ => <div />
         }}
      </SLayout>
    }
  />;
};