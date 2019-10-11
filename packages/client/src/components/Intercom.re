[@bs.module "../lib/intercom.js"]
external intercom: (string, string, string, option(MomentRe.Moment.t)) => unit =
  "intercom";

let component = ReasonReact.statelessComponent("Intercom");
let make = _children => {
  ...component,
  render: _self => {
    <Providers.AppContext.Consumer>
      ...{(context: Providers.appContext) => {
        let loggedInUser = context.loggedInUser;

        switch (Env.clientEnv, loggedInUser) {
        | (Production, Some(loggedInUser)) =>
          let name = loggedInUser.name == "" ? "User" : loggedInUser.name;
          let email =
            loggedInUser.email |> E.O.default("no-reply@foretold.io");
          let createdAt = loggedInUser.createdAt;

          intercom(Env.intercomAppId, name, email, createdAt);
        | _ => ()
        };

        ReasonReact.null;
      }}
    </Providers.AppContext.Consumer>;
  },
};
