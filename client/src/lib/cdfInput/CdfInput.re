type action =
  | UpdateFloatPdf((array(float), array(float)));

type state = {floatPdf: (array(float), array(float))};

let component = ReasonReact.reducerComponent("CdfInput");

let make = _children => {
  ...component,
  initialState: () => {floatPdf: ([||], [||])},
  reducer: (action, _) =>
    switch (action) {
    | UpdateFloatPdf(e) => ReasonReact.Update({floatPdf: e})
    },
  render: ({state, send}) =>
    <div>
      <DynamicForm
        sampleCount=10000
        onUpdate=(e => send(UpdateFloatPdf(e)) |> ignore)
      />
      <CellInputChart
        data=(
          state.floatPdf
          |> Value.FloatCdf.fromArrays
          |> Value.FloatCdf.toPoints
        )
      />
    </div>,
};