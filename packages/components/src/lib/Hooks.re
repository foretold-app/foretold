type size = {
  .
  "width": int,
  "height": int,
};

[@bs.module "react-use"]
external useSize:
  (size => ReasonReact.reactElement, size) => (React.element, size) =
  "useSize";