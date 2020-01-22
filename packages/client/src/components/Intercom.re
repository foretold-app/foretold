[@bs.module "../lib/js/intercom.js"]
external intercom:
  (string, string, string, string, option(MomentRe.Moment.t)) => unit =
  "intercom";

[@react.component]
let make = () => {
  let context = React.useContext(Providers.app);

  switch (Env.clientEnv, context.loggedUser) {
  | (Production, Some(loggedUser)) =>
    let name = loggedUser |> Primary.User.getName;
    let email = loggedUser.email |> E.O.default("no-reply@foretold.io");
    let createdAt = loggedUser.createdAt;
    let userId = loggedUser.id;

    intercom(Env.intercomAppId, name, email, userId, createdAt);
  | _ => ()
  };

  <Null />;
};