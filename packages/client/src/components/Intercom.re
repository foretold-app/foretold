[@bs.module "../lib/intercom.js"]
external intercom:
  (string, string, string, string, option(MomentRe.Moment.t)) => unit =
  "intercom";

let component = ReasonReact.statelessComponent("Intercom");
let make = _children => {
  ...component,
  render: _self => {
    <Providers.AppContext.Consumer>
      ...{({loggedUser}) => {
        switch (Env.clientEnv, loggedUser) {
        | (Production, Some(loggedUser)) =>
          let name = loggedUser |> Primary.User.getName;
          let email = loggedUser.email |> E.O.default("no-reply@foretold.io");
          let createdAt = loggedUser.createdAt;
          let userId = loggedUser.id;

          intercom(Env.intercomAppId, name, email, userId, createdAt);
        | _ => ()
        };

        ReasonReact.null;
      }}
    </Providers.AppContext.Consumer>;
  },
};