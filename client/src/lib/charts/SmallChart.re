open Utils;

let component = ReasonReact.statelessComponent("TableChart");

let make = (~data, _children) => {
  ...component,
  render: _ =>
    <div>
      Victory.(
        <div>
          <VictoryChart
            padding={"top": 1, "bottom": 9, "right": 1, "left": 3}
            height=40
            width=150>
            <VictoryAxis
              height=200
              scale={"x": "linear", "y": "log"}
              style={
                "axis": {
                  "stroke": "#aaa",
                },
                "tickLabels": {
                  "fontSize": 7,
                  "padding": 2,
                },
              }
            />
            <VictoryLine data style={
                                "data": {
                                  "stroke": "#aaa",
                                },
                              } />
          </VictoryChart>
        </div>
      )
    </div>,
};