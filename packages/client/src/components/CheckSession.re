[@bs.config {jsx: 3}];

let component = ReasonReact.statelessComponent("CheckSession");

let make = _children => {
  ...component,
  render: _self => {
    <Providers.AppContext.Consumer>
      ...{({loggedUser}) =>
        switch (loggedUser) {
        | Some(loggedUser) =>
          <UserAccessTokenUpdate.Mutation>
            ...{(mutation, data) =>
              switch (data.result) {
              | NotCalled =>
                Auth0Client.checkSession((authResult: Auth0Client.authResult) => {
                  let accessToken = authResult##accessToken;
                  let idToken = authResult##idToken;
                  let expiresInSec = authResult##expiresIn;

                  UserAccessTokenUpdate.mutate(
                    mutation,
                    loggedUser.id,
                    accessToken,
                  );

                  let nowInSec =
                    MomentRe.momentNow() |> MomentRe.Moment.toUnix;
                  let expiresAt = nowInSec + expiresInSec;
                  let expiresAtStr = string_of_int(expiresAt);

                  Auth0Tokens.make(accessToken, idToken, expiresAtStr)
                  |> Auth0Tokens.set;

                  ();
                });

                ReasonReact.null;
              | _ => ReasonReact.null
              }
            }
          </UserAccessTokenUpdate.Mutation>
        | _ => ReasonReact.null
        }
      }
    </Providers.AppContext.Consumer>;
  },
};