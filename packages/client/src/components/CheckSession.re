let component = ReasonReact.statelessComponent("CheckSession");

let make = _children => {
  ...component,
  render: _self => {
    <Providers.AppContext.Consumer>
      ...{({loggedInUser}) =>
        switch (loggedInUser) {
        | Some(loggedInUser) =>
          <UserUpdate.EditUserMutation>
            ...{(mutation, data) =>
              switch (data.result) {
              | NotCalled =>
                let accessToken = "accessToken1";
                let idToken = "idToken1";
                let authResult = "authResult1";

                Js.log2("accessToken", accessToken);
                Js.log2("idToken", idToken);
                Js.log2("authResult", authResult);

                UserUpdate.mutateAccessToken(
                  mutation,
                  loggedInUser.id,
                  accessToken,
                );

                ReasonReact.null;
              | _ => ReasonReact.null
              }
            }
          </UserUpdate.EditUserMutation>
        | _ => ReasonReact.null
        }
      }
    </Providers.AppContext.Consumer>;
  },
};
