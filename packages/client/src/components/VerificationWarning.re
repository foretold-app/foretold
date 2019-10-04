let component = ReasonReact.statelessComponent("Banner");

module Styles = {
  open Css;
  let icon = style([marginRight(`px(4))]);
};

let make = _ => {
  ...component,
  render: _ => {
    <>
      <Providers.AppContext.Consumer>
        ...{({loggedInUser}) => {
          let warning = email =>
            <>
              <span className=Styles.icon>
                <Icon.Icon icon="EMAIL_UNREAD" />
              </span>
              {"Please verify your email \""
               ++ email
               ++ "\" address in Auth0.com and "
               |> Utils.ste}
              <Link.Jsx2 linkType={Action(_ => Auth.Actions.logout())}>
                {"relogin after" |> Utils.ste}
              </Link.Jsx2>
              {"." |> Utils.ste}
            </>;

          let warningNoEmail =
            <>
              <span className=Styles.icon>
                <Icon.Icon icon="EMAIL_UNREAD" />
              </span>
              {"Please specify and vefiry your email address in Auth0.com and "
               |> Utils.ste}
              <Link.Jsx2 linkType={Action(_ => Auth.Actions.logout())}>
                {"relogin after" |> Utils.ste}
              </Link.Jsx2>
              {"." |> Utils.ste}
            </>;

          switch (loggedInUser) {
          | Some({
              isEmailVerified: Some(isEmailVerified),
              email: Some(email),
            }) =>
            isEmailVerified ? ReasonReact.null : warning(email)
          | _ => warningNoEmail
          };
        }}
      </Providers.AppContext.Consumer>
    </>;
  },
};
