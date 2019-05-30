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
  type jsProps = {title: string};
  let make = (~title, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=rcSubMenuClass,
      ~props=jsProps(~title),
      children,
    );
};

module Item = {
  // https://github.com/react-component/menu#menuitem-props
  [@bs.deriving abstract]
  type jsProps = {
    disabled: bool,
    itemIcon: Js.Undefined.t(React.element),
  };
  let make = (~disabled=false, ~itemIcon=None, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=rcItemClass,
      ~props=jsProps(~disabled, ~itemIcon=Js.Undefined.fromOption(itemIcon)),
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
};

type clickEvent = {
  key: string,
  item: React.element,
  domEvent: Dom.event,
  keyPath: array(string),
};

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
      ),
    children,
  );
