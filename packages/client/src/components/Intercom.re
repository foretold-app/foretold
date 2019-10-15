[@bs.module "../lib/intercom.js"]
external intercom:
  (string, string, string, string, option(MomentRe.Moment.t)) => unit =
  "intercom";

let component = ReasonReact.statelessComponent("Intercom");
let make = _children => {
  ...component,
  render: _self => {
    <Providers.AppContext.Consumer>
      ...{({loggedInUser}) => {
        switch (Env.clientEnv, loggedInUser) {
        | (Production, Some(loggedInUser)) =>
          let name = loggedInUser |> Primary.User.getName;
          let email =
            loggedInUser.email |> E.O.default("no-reply@foretold.io");
          let createdAt = loggedInUser.createdAt;
          let userId = loggedInUser.id;

          intercom(Env.intercomAppId, name, email, userId, createdAt);
        | _ => ()
        };

        ReasonReact.null;
      }}
    </Providers.AppContext.Consumer>;
  },
};
