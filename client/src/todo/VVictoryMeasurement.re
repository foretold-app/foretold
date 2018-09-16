open Utils;
let component = ReasonReact.statelessComponent("VVictoryMeasurement");

/* I have not been able to get this to work yet. See VictoryMeasurement.js instead.*/
let make =
    (
      ~scale=?,
      ~point: {
         .
         "x": 'a,
         "y1": 'b,
         "y2": 'b,
         "y3": 'b,
       },
      _children,
    ) => {
  ...component,
  render: _ =>
    switch (scale) {
    | Some(sca) =>
      let xx = sca##x(point##x);
      <svg>
        <line
          x1=xx
          y1=(sca##y(point##y1))
          x2=xx
          y2=(sca##y(point##y3))
          stroke="#000"
        />
      </svg>;
    | None => <div />
    },
};