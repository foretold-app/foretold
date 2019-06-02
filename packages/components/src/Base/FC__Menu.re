/**
 * Menu component and sibling components provides define menu items,
 * submenues and dividers. See make for usage.
 *
 * It is binding to https://github.com/react-component/menu
 *
 * Notable unimplemented things are selection of one or multiple items.
 */
[@bs.module "rc-menu"]
external rcMenuClass: ReasonReact.reactClass = "default";
[@bs.module "rc-menu"]
external rcSubMenuClass: ReasonReact.reactClass = "SubMenu";
[@bs.module "rc-menu"] external rcItemClass: ReasonReact.reactClass = "Item";
[@bs.module "rc-menu"]
external rcDividerClass: ReasonReact.reactClass = "Divider";

[%bs.raw {|require("rc-menu/assets/index.css")|}];

module SubMenu = {
  // https://github.com/react-component/menu#menusubmenu-props
  [@bs.deriving abstract]
  type jsProps = {title: string, popupClassName:string};
  let make = (~title, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=rcSubMenuClass,
      ~props=jsProps(~title, ~popupClassName="custom-submenu"),
      children,
    );
};

module Item = {
  // https://github.com/react-component/menu#menuitem-props
  [@bs.deriving abstract]
  type jsProps = {
    disabled: bool,
    itemIcon: Js.Undefined.t(React.element),
    className: string,
  };
  let make = (~disabled=false, ~itemIcon=None, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=rcItemClass,
      ~props=
        jsProps(
          ~disabled,
          ~itemIcon=Js.Undefined.fromOption(itemIcon),
          ~className="custom-menu-item",
        ),
      children,
    );
};

module Divider = {
  // https://github.com/react-component/menu#menuitem-props
  let make = children =>
    ReasonReact.wrapJsForReason(
      ~reactClass=rcDividerClass,
      ~props=Js.Obj.empty(),
      children,
    );
};

// https://github.com/react-component/menu#menu-props
[@bs.deriving abstract]
type jsProps = {
  [@bs.as "onClick"]
  jsOnClick:
    {
      .
      "key": string,
      "item": React.element,
      "domEvent": Dom.event,
      "keyPath": array(string),
    } =>
    unit,
  selectable: bool,
  className: string,
};

type clickInfo = {
  key: string,
  item: React.element,
  domEvent: Dom.event,
  keyPath: array(string),
};

/**
 * Menu component and sibling components provides a way to define menu items,
 * submenues and dividers. One can register an onClick callback on the
 * <Menu> element to receive information about clicks.
 *
 * To resolve which menu item was clicked, the best way is probably
 * to provide a key attribute to the items, then use the key in
 * the callback. There is also a keyPath list with parents keys
 * in the case of a submenu, the dom Event and the react element
 * that was clicked.
 *
 * Usage:
 * ```
 * Menu.(
 *   <Menu onClick={info => Js.log(info.key)}>
 *     <Item key="item1"> { "Item 1" |> React.string } </Item>
 *     <SubMenu title="Submenu">
 *       <Item key="subitem1"> { "Subitem 1" |> React.string } </Item>
 *       <Divider />
 *       <Item key="subitem2"> { "Subitem 2" |> React.string } </Item>
 *     </SubMenu>
 *   </Menu>
 * )
 * ```
 */
let make = (~onClick=?, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=rcMenuClass,
    ~props=
      jsProps(
        ~jsOnClick=
          info =>
            switch (onClick) {
            | Some(onClick) =>
              onClick({
                key: info##key,
                item: info##item,
                domEvent: info##domEvent,
                keyPath: info##keyPath,
              })
            | None => ()
            },
        ~selectable=false,
        ~className="custom-menu",
      ),
    children,
  ) /* Menu item group is not implemented (https://react-component.github.io/menu/examples/menuItemGroup.html*/;
