module Styles = {
  open Css;
  let icon = style([marginRight(`px(4))]);
};

[@react.component]
let make = _ => {
  <Providers.AppContext.Consumer>
    ...{({loggedUser}) => {
      let warning = email =>
        <ForetoldComponents.Alert type_=`warning>
          <span className=Styles.icon>
            <Icon icon="EMAIL_UNREAD" />
          </span>
          {"Please verify your email address, \""
           ++ email
           ++ "\"."
           |> Utils.ste}
        </ForetoldComponents.Alert>;

      let warningNoEmail =
        <ForetoldComponents.Alert type_=`warning>
          <span className=Styles.icon>
            <Icon icon="EMAIL_UNREAD" />
          </span>
          {"Please verify your email address. " |> Utils.ste}
        </ForetoldComponents.Alert>;

      switch (loggedUser) {
      | Some({isEmailVerified: Some(isEmailVerified), email: Some(email)}) =>
        isEmailVerified ? <Null /> : warning(email)
      | Some({isEmailVerified: _, email: None}) => warningNoEmail
      | _ => <Null />
      };
    }}
  </Providers.AppContext.Consumer>;
};