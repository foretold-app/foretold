open Utils;

let component = ReasonReact.statelessComponent("Chart");

let make = (~bounds, _children) => {
  ...component,
  render: _ => {
    let (low, high) = bounds;
    Victory.(
      <VictoryChart
        padding={"top": 0, "bottom": 25, "right": 5, "left": 5}
        height=25
        maxDomain={"x": high}
        minDomain={"x": low}>
        <VictoryAxis
          scale={"x": "linear", "y": "log"}
          style={
            "axis": {
              "stroke": "rgba(0,0,0,0)",
            },
            "ticks": {
              "stroke": "rgba(110, 136, 144, 0.4)",
              "size": 3,
            },
            "tickLabels": {
              "fontSize": 14,
              "padding": 5,
              "fill": "rgb(110, 136, 144)",
            },
          }
        />
      </VictoryChart>
    );
  },
};