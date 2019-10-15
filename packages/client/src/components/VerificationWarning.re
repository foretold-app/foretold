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
            <FC__Alert type_=`warning>
              <span className=Styles.icon>
                <Icon.Icon icon="EMAIL_UNREAD" />
              </span>
              {"Please verify your email address, \""
               ++ email
               ++ "\"."
               |> Utils.ste}
            </FC__Alert>;

          let warningNoEmail =
            <FC__Alert type_=`warning>
              <span className=Styles.icon>
                <Icon.Icon icon="EMAIL_UNREAD" />
              </span>
              {"Please verify your email address. " |> Utils.ste}
            </FC__Alert>;

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
