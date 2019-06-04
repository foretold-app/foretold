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

module Styles = {
  open Css;
  let menuBorderRadius = `px(3);
  // Menu general, applied to menu <ul> elements
  global(".ft-menu-general, .ft-submenu-general>ul", [
      fontFamily("Lato"),
      listStyleType(`none),
      position(`relative),
      outlineStyle(`none),
      borderRadius(menuBorderRadius),
      borderStyle(`none),
      boxShadow(~x=`zero, ~y=`px(2), ~blur=`px(8), `rgba((0, 0, 0, 0.15))),
      margin(`zero),
      padding2(~v=`px(4), ~h=`zero),
      // This is applied to menu-items, submenu pointers, and dividers
      selector(">li", [
        // Used to place icon in submenu item currently
        position(`relative),
        display(`block),
        clear(`both),
        whiteSpace(`nowrap),
        cursor(`default)
      ]),
      // Taken from default stylesheet, might prevent some small bug
      // with border radius
      selector(">li.ft-menu-item-general:first-child", [
        borderTopLeftRadius(menuBorderRadius),
        borderTopRightRadius(menuBorderRadius)
      ])
  ]);

  // Applied to div of submenu
  global(".ft-submenu-general", [
    position(`absolute),
    minWidth(`px(100))
  ]);
};

module SubMenu = {
  // https://github.com/react-component/menu#menusubmenu-props
  [@bs.deriving abstract]
  type jsProps = {title: string, popupClassName:string};
  let make = (~title, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=rcSubMenuClass,
      ~props=jsProps(~title, ~popupClassName="ft-submenu-general"),
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
          ~className="ft-menu-item-general",
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
        ~className="ft-menu-general",
      ),
    children,
  ) /* Menu item group is not implemented (https://react-component.github.io/menu/examples/menuItemGroup.html*/;
