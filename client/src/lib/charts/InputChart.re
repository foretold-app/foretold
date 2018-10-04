open Utils;

let component = ReasonReact.statelessComponent("Chart");

let make = (~data, _children) => {
  ...component,
  render: _ =>
    <div>
      Victory.(
        <div>
          <VictoryChart>
            <VictoryAxis
              style={
                "axis": {
                  "stroke": "#aaa",
                },
                "tickLabels": {
                  "fontSize": "7",
                  "padding": "2",
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