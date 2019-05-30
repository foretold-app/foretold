open FC.Base;

let staticOverlay = <div> {"Overlay" |> React.string} </div>;

let entries =
  EntryTypes.[
    entry(~title="Dropdown1", ~render=() =>
      <Dropdown overlay=staticOverlay trigger=Dropdown.Click>
        <div> "Dropdown"->React.string </div>
      </Dropdown>
    ),
  ];
