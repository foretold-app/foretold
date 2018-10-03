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
      <Victory.VictoryChart>
        <Victory.VictoryLine
          data=(
            state.floatPdf
            |> Value.FloatCdf.fromArrays
            /* |> Value.toPdf(~bucketSize=100) */
            |> Value.FloatCdf.toPoints
          )
        />
      </Victory.VictoryChart>
      <DynamicForm
        sampleCount=1000
        cdfCount=1
        onUpdate=(e => send(UpdateFloatPdf(e)) |> ignore)
      />
    </div>,
};