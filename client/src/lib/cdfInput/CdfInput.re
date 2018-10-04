open Types;

type action =
  | UpdateFloatPdf(floatCdf);

type state = {floatCdf};

let component = ReasonReact.reducerComponent("CdfInput");

let make = (~onUpdate=e => (), _children) => {
  ...component,
  initialState: () => {floatCdf: floatCdfEmpty},
  reducer: (action, _) =>
    switch (action) {
    | UpdateFloatPdf((e: floatCdf)) =>
      onUpdate(e);
      ReasonReact.Update({floatCdf: e});
    },
  render: ({state, send}) =>
    <div>
      <GuesstimateInput
        sampleCount=1000
        onUpdate=(
          e =>
            {
              let (ys, xs) = e;
              let asGroup: floatCdf = {xs, ys};
              send(UpdateFloatPdf(asGroup));
            }
            |> ignore
        )
      />
      <InputChart
        data=(
          state.floatCdf
          |> (e => (e.xs, e.ys))
          |> Value.FloatCdf.fromArrays
          |> Value.FloatCdf.toPoints
        )
      />
    </div>,
};