[@react.component]
let make = () => {
  let context = React.useContext(Providers.app);
  switch (context.loggedUser) {
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

            let nowInSec = MomentRe.momentNow() |> MomentRe.Moment.toUnix;
            let expiresAt = nowInSec + expiresInSec;
            let expiresAtStr = string_of_int(expiresAt);

            Auth0Tokens.make(accessToken, idToken, expiresAtStr)
            |> Auth0Tokens.set;

            ();
          });

          <Null />;
        | _ => <Null />
        }
      }
    </UserAccessTokenUpdate.Mutation>
  | _ => <Null />
  };
};