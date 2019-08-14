type appContext = {
  route: Routing.Route.t,
  authToken: option(string),
  me: option(Me.t),
  loggedInUser: option(Types.user),
  globalSetting: option(Types.globalSetting),
};

module AppContext =
  Contexts.MakePair({
    type t = appContext;
    let defaultValue = {
      route: Home,
      authToken: None,
      me: None,
      loggedInUser: None,
      globalSetting: None,
    };
  });