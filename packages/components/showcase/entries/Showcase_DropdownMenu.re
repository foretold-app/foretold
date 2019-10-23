open FC.Base;

let simpleMenu = () =>
  <DropdownMenu.Jsx2 title="Simple menu">
    Menu.(
      <Menu.Jsx2 onClick={e => Js.log(e.key)}>
        <Item.Jsx2 key="item1"> "1st menu item"->React.string </Item.Jsx2>
        <Item.Jsx2 key="item2"> "2nd menu item"->React.string </Item.Jsx2>
        <Divider.Jsx2 />
        <Item.Jsx2 key="item3"> "3nd menu item"->React.string </Item.Jsx2>
      </Menu.Jsx2>
    )
  </DropdownMenu.Jsx2>;

let subMenu = () =>
  <DropdownMenu.Jsx2 title="Submenu" trigger=Dropdown.Hover>
    Menu.(
      <Menu.Jsx2 onClick={e => Js.log(e.key)}>
        <Item.Jsx2 key="item1"> "Item1"->React.string </Item.Jsx2>
        <Divider.Jsx2 />
        <Item.Jsx2 key="item2"> "Item2"->React.string </Item.Jsx2>
        <SubMenu.Jsx2 title="Submenu1">
          <Item.Jsx2 key="item3"> "Item3"->React.string </Item.Jsx2>
          <Item.Jsx2 key="item4"> "Item4"->React.string </Item.Jsx2>
        </SubMenu.Jsx2>
      </Menu.Jsx2>
    )
  </DropdownMenu.Jsx2>;

let entries =
  EntryTypes.[
    folder(
      ~title="Dropdown menu",
      ~children=[
        entry(~title="Simple", ~render=simpleMenu),
        entry(~title="Submenu", ~render=subMenu),
      ],
    ),
  ];
