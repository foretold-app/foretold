module Styles = {
  open Css;
  let icon = style([marginRight(`px(4))]);
};

[@react.component]
let make = _ => {
  let context = React.useContext(Providers.app);

  let warning = email =>
    <ForetoldComponents.Alert type_=`warning>
      <span className=Styles.icon> <Icon icon="EMAIL_UNREAD" /> </span>
      {"Please verify your email address, \"" ++ email ++ "\"." |> Utils.ste}
    </ForetoldComponents.Alert>;

  let warningNoEmail =
    <ForetoldComponents.Alert type_=`warning>
      <span className=Styles.icon> <Icon icon="EMAIL_UNREAD" /> </span>
      {"Please verify your email address. " |> Utils.ste}
    </ForetoldComponents.Alert>;

  switch (context.loggedUser) {
  | Some({isEmailVerified: Some(isEmailVerified), email: Some(email)}) =>
    isEmailVerified ? <Null /> : warning(email)
  | Some({isEmailVerified: _, email: None}) => warningNoEmail
  | _ => <Null />
  };
};