[@bs.config {jsx: 3}];

module Styles = {
  open Css;
  let icon = style([marginRight(`px(4))]);
};

[@react.component]
let make = _ => {
  <Providers.AppContext.Consumer>
    ...{({loggedUser}) => {
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

      switch (loggedUser) {
      | Some({isEmailVerified: Some(isEmailVerified), email: Some(email)}) =>
        isEmailVerified ? ReasonReact.null : warning(email)
      | Some({isEmailVerified: _, email: None}) => warningNoEmail
      | _ => ReasonReact.null
      };
    }}
  </Providers.AppContext.Consumer>;
};
