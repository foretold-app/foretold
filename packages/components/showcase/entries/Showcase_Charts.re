open FC.Base;

let chart1 = () => <div> <RePercentilesChart /> </div>;

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