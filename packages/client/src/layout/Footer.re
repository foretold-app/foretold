open Utils;
let component = ReasonReact.statelessComponent("Footer");

let documentationUrl = "https://docs.foretold.io/";
let feedbackUrl = "https://feedback.foretold.io/";

let make = _children => {
  ...component,
  render: _self =>
    <Fc.FC__Footer
      logo={React.string({js|Foretold|js})}
      links=[|
        <Link.Jsx2 linkType={External(documentationUrl)}>
          {"Documentation" |> ste}
        </Link.Jsx2>,
        <Link.Jsx2 linkType={External(feedbackUrl)}>
          {"Feedback" |> ste}
        </Link.Jsx2>,
        <Link.Jsx2 linkType={Internal(Terms)}>
          {"Terms and Conditions" |> ste}
        </Link.Jsx2>,
        <Link.Jsx2 linkType={Internal(Privacy)}>
          {"Privacy Policy" |> ste}
        </Link.Jsx2>,
      |]
    />,
};