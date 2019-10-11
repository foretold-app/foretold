[@bs.module "../lib/intercom.js"]
external intercom: (string, string, string, option(MomentRe.Moment.t)) => unit =
  "intercom";

let component = ReasonReact.statelessComponent("CheckSession");
let make = _children => {
  ...component,
  render: _self => {
    <Providers.AppContext.Consumer>
      ...{({loggedInUser}) =>
        switch (loggedInUser) {
        | Some(loggedInUser) =>
          Auth0Client.checkSession(authResult => {
            let accessToken = authResult##accessToken;
            let idToken = authResult##idToken;

            Js.log(accessToken);
            Js.log(idToken);
            Js.log(authResult);

            <UserUpdate.EditUserMutation>
              ...{mutation => {
                UserUpdate.mutateAccessToken(
                  mutation,
                  loggedInUser.id,
                  accessToken,
                );
                ReasonReact.null;
              }}
            </UserUpdate.EditUserMutation>;
          })
        | _ => ReasonReact.null
        }
      }
    </Providers.AppContext.Consumer>;
  },
};
