let component = ReasonReact.statelessComponent("Banner");
let make = _ => {
  ...component,
  render: _ => {
    let warning =
      <>
        {"Please verify your email address with Auth0.com and " |> Utils.ste}
        <Link.Jsx2 linkType={Action(_ => Auth.Actions.logout())}>
          {"relogin after" |> Utils.ste}
        </Link.Jsx2>
        {"." |> Utils.ste}
      </>;
    <Providers.AppContext.Consumer>
      ...{({loggedInUser}) =>
        switch (loggedInUser) {
        | Some({isEmailVerified: Some(isEmailVerified)}) =>
          isEmailVerified ? ReasonReact.null : warning
        | _ => warning
        }
      }
    </Providers.AppContext.Consumer>;
  },
};
