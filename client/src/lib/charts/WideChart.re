open Utils;

let component = ReasonReact.statelessComponent("Chart");

let make = (~data, ~bounds, _children) => {
  ...component,
  render: _ => {
    let (low, high) = bounds;
    Victory.(
      <VictoryChart
        padding={"top": 5, "bottom": 0, "right": 5, "left": 5}
        height=20
        maxDomain={"x": high}
        minDomain={"x": low}>
        <VictoryAxis
          scale={"x": "linear", "y": "log"}
          style={
            "axis": {
              "stroke": "rgba(255,255,255,0)",
            },
            "grid": {
              "stroke": "rgba(0, 0, 0, 0.0)",
            },
            "tickLabels": {
              "fontSize": 0,
              "padding": 0,
              "fill": "#ddd",
            },
          }
        />
        <VictoryArea
          data
          style={
            "data": {
              "fill": "rgb(188, 204, 212)",
              "strokeWidth": "1",
              "width": "10px",
            },
          }
        />
      </VictoryChart>
    );
  },
};