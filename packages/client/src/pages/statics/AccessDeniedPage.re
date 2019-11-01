[@bs.config {jsx: 3}];

[@react.component]
let make = () => {
  <SLayout head=ReasonReact.null isFluid=true>
    {"Sorry, but an access is denied." |> Utils.ste}
  </SLayout>;
};