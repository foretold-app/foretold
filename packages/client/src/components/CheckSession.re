let component = ReasonReact.statelessComponent("CheckSession");
let make = _children => {
  ...component,
  render: _self => {
    <Providers.AppContext.Consumer>
      ...{({loggedInUser}) =>
        switch (loggedInUser) {
        | Some(loggedInUser) =>
          Js.log("Rest 1");
          UserUpdate.withUserMutation(mutation => {
            Js.log("withUserMutation 2");

            let accessToken = "accessToken1";
            let idToken = "idToken1";
            let authResult = "authResult1";

            //            Auth0Client.checkSession(authResult => {
            //              let accessToken = authResult##accessToken;
            //              let idToken = authResult##idToken;

            Js.log2("accessToken", accessToken);
            Js.log2("idToken", idToken);
            Js.log2("authResult", authResult);

            let mutate =
              UserUpdate.Query.make(
                ~id=loggedInUser.id,
                ~input={
                  "name": None,
                  "email": None,
                  "picture": None,
                  "description": None,
                  "auth0AccessToken": Some(accessToken),
                },
                (),
              );

            mutation(~variables=mutate##variables, ~refetchQueries=[||], ())
            |> ignore;

            //            });

            (
              rest => {
                Js.log2("Rest", rest);
                ReasonReact.null;
              }
            );
          });
        | _ => ReasonReact.null
        }
      }
    </Providers.AppContext.Consumer>;
  },
};
