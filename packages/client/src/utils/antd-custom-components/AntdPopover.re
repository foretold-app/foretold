[@bs.config {jsx: 3}];

[@bs.module] external reactClass: ReasonReact.reactClass = "antd/lib/popover";

[%bs.raw {|require("antd/lib/popover/style")|}];

[@bs.deriving jsConverter]
type placementType = [
  | `top
  | `left
  | `right
  | `bottom
  | `topLeft
  | `topRight
  | `bottomLeft
  | `bottomRight
  | `leftTop
  | `leftBottom
  | `rightTop
  | `rightBottom
];

[@bs.deriving jsConverter]
type triggerType = [ | `hover | `focus | `click | `contextMenu];

/*
 COMMON API
 -----------------------
 arrowPointAtCenter	Whether the arrow is pointed at the center of target, supported after antd@1.11+	boolean	false
 autoAdjustOverflow	Whether to adjust popup placement automatically when popup is off screen	boolean	true
 defaultVisible	Whether the floating tooltip card is visible by default	boolean	false
 getPopupContainer	The DOM container of the tip, the default behavior is to create a div element in body	Function(triggerNode)	() => document.body
 mouseEnterDelay	Delay in seconds, before tooltip is shown on mouse enter	number	0
 mouseLeaveDelay	Delay in seconds, before tooltip is hidden on mouse leave	number	0.1
 overlayClassName	Class name of the tooltip card	string	-
 overlayStyle	Style of the tooltip card	object	-
 placement	The position of the tooltip relative to the target, which can be one of top left right bottom topLeft topRight bottomLeft bottomRight leftTop leftBottom rightTop rightBottom	string	top
 trigger	Tooltip trigger mode	hover | focus | click | contextMenu	hover
 visible	Whether the floating tooltip card is visible or not	boolean	false
 onVisibleChange	Callback executed when visibility of the tooltip card is changed	(visible) => void	-
 align	this value will be merged into placement's config, please refer to the settings rc-tooltip	Object	-
 */

/*
 content	Content of the card	string|ReactNode	-
 title	Title of the card	string|ReactNode	-
 */

[@bs.obj]
external makeProps:
  (
    ~arrowPointAtCenter: bool=?,
    ~autoAdjustOverflow: bool=?,
    ~defaultVisible: bool=?,
    ~getPopupContainer: Dom.element => Dom.htmlElement=?,
    ~mouseEnterDelay: float=?,
    ~mouseLeaveDelay: float=?,
    ~overlayClassName: string=?,
    ~overlayStyle: ReactDOMRe.Style.t=?,
    ~placement: string=?,
    ~trigger: string=?,
    ~visible: bool=?,
    ~onVisibleChange: bool => unit=?,
    ~content: ReasonReact.reactElement=?,
    ~title: ReasonReact.reactElement=?,
    ~id: string=?,
    ~className: string=?,
    ~style: ReactDOMRe.Style.t=?,
    unit
  ) =>
  _ =
  "";

[@react.component]
let make =
    (
      ~arrowPointAtCenter=?,
      ~autoAdjustOverflow=?,
      ~defaultVisible=?,
      ~getPopupContainer=?,
      ~mouseEnterDelay=?,
      ~mouseLeaveDelay=?,
      ~overlayClassName=?,
      ~overlayStyle=?,
      ~placement=?,
      ~trigger=?,
      ~visible=?,
      ~onVisibleChange=?,
      ~content=?,
      ~title=?,
      ~id=?,
      ~className=?,
      ~style=?,
      ~children=<Null />,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=
      makeProps(
        ~arrowPointAtCenter?,
        ~autoAdjustOverflow?,
        ~defaultVisible?,
        ~getPopupContainer?,
        ~mouseEnterDelay?,
        ~mouseLeaveDelay?,
        ~overlayClassName?,
        ~overlayStyle?,
        ~placement=?Js.Option.map((. b) => placementTypeToJs(b), placement),
        ~trigger=?Js.Option.map((. b) => triggerTypeToJs(b), trigger),
        ~visible?,
        ~onVisibleChange?,
        ~content?,
        ~title?,
        ~id?,
        ~className?,
        ~style?,
        (),
      ),
    children,
  )
  |> ReasonReact.element;