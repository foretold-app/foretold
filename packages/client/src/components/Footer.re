open Utils;

let documentationUrl = "https://docs.foretold.io/";

[@react.component]
let make = () => {
  <ForetoldComponents.Footer
    logo={React.string({js|Foretold.io|js})}
    links=[|
      <Link linkType={External(documentationUrl)}>
        {"Documentation" |> ste}
      </Link>,
      <Link linkType={Internal(Terms)}>
        {"Terms and Conditions" |> ste}
      </Link>,
      <Link linkType={Internal(Privacy)}> {"Privacy Policy" |> ste} </Link>,
    |]
  />;
};