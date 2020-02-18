open FC.Base;

[@bs.module] external data: Js.Json.t = "./sample-measurements.json";

let chart1 = () => <div> <RePercentilesChart data /> </div>;

let chart2 = () => <div> "Chart 2"->React.string </div>;

let entries =
  EntryTypes.[
    folder(
      ~title="Charts",
      ~children=[
        entry(~title="Chart 1", ~render=chart1),
        entry(~title="Chart 2", ~render=chart2),
      ],
    ),
  ];