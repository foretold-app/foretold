type appContext = {
  authToken: option(string),
  me: option(Me.t),
  loggedInUser: option(Types.user),
};

module AppContext =
  Contexts.MakePair({
    type t = appContext;
    let defaultValue = {authToken: None, me: None, loggedInUser: None};
  });