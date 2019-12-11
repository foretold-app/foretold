open Utils;
open E;

[@react.component]
let make = () => {
  <div className=StaticStyles.body>
    <div className=StaticStyles.title> {"Foretold.io" |> ste} </div>
    <div className=StaticStyles.description>
      {"Foretold.io is currently in private beta. Contact  " |> ste}
      <a href="mailto:ozzieagooen@gmail.com"> {"Ozzie Gooen" |> ste} </a>
      {" for early access." |> ste}
    </div>
  </div>;
};