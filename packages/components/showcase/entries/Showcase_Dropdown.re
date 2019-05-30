open FC.Base;

let staticOverlay = <div className=Css.(style([border(`px(1), `solid, Colors.grey1)]))> {"Overlay" |> React.string} </div>;

let entries =
  EntryTypes.[
    entry(~title="Dropdown1", ~render=() =>
      <Dropdown overlay=staticOverlay trigger=Dropdown.Click>
        <div> "Dropdown"->React.string </div>
      </Dropdown>
    ),
  ];
