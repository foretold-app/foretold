let component = ReasonReact.statelessComponent("Banner");
let make = _ => {
  ...component,
  render: _ =>
    <Providers.AppContext.Consumer>
      ...{({loggedInUser}) =>
        switch (loggedInUser) {
        | Some({isEmailVerified: Some(isEmailVerified)}) =>
          isEmailVerified
            ? ReasonReact.null
            : "Please verify your email address with Auth0.com." |> Utils.ste
        | _ => ReasonReact.null
        }
      }
    </Providers.AppContext.Consumer>,
};
