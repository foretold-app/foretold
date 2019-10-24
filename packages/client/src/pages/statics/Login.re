[@bs.config {jsx: 3}];

open Utils;

// @todo: 1

[@react.component]
let make = () => {
  <div className=StaticStyles.body>
    <div className=StaticStyles.title> {"Foretold.io" |> ste} </div>
    <div className=StaticStyles.description>
      {"<Antd.Button
        _type=`primary onClick={_e => Auth0Client.triggerLoginScreen()}>
        {\"Login\" |> ste}
      </Antd.Button>"
       |> Utils.ste}
    </div>
  </div>;
};
