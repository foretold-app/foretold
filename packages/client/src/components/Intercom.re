[@bs.module "../lib/intercom.js"]
external intercom: (string, string) => unit = "intercom";

let component = ReasonReact.statelessComponent("Intercom");
let make = _children => {
  ...component,
  render: _self => {
    <Providers.AppContext.Consumer>
      ...{(context: Providers.appContext) => {
        let loggedInUser = context.loggedInUser;

        switch (Env.prod, loggedInUser) {
        | (false, Some(loggedInUser)) =>
          let name = loggedInUser.name == "" ? "User" : loggedInUser.name;
          let email =
            loggedInUser.email |> E.O.default("no-reply@foretold.io");

          intercom(name, email);
        | _ => ()
        };

        ReasonReact.null;
      }}
    </Providers.AppContext.Consumer>;
  },
};
