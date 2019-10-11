type appContext = {
  route: Routing.Route.t,
  authToken: option(string),
  loggedInUser: option(Types.user),
  globalSetting: option(Types.globalSetting),
};

module AppContext =
  Contexts.MakePair({
    type t = appContext;
    let defaultValue = {
      route: Home,
      authToken: None,
      loggedInUser: None,
      globalSetting: None,
    };
  });
