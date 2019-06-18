open Utils;
let component = ReasonReact.statelessComponent("Footer");

let documentationUrl = "https://docs.foretold.io/";
let feedbackUrl = "https://feedback.foretold.io/";

let make = _children => {
  ...component,
  render: _self =>
    <FC__Footer
      logo={React.string({js|Foretold|js})}
      links=[|
        <C.Link linkType={External(documentationUrl)}>
          {"Documentation" |> ste}
        </C.Link>,
        <C.Link linkType={External(feedbackUrl)}>
          {"Feedback" |> ste}
        </C.Link>,
        <C.Link linkType={Internal(Terms)}>
          {"Terms and Conditions" |> ste}
        </C.Link>,
        <C.Link linkType={Internal(Privacy)}>
          {"Privacy Policy" |> ste}
        </C.Link>,
      |]
    />,
};