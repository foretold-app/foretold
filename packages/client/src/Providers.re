[@bs.config {jsx: 3}];

type appContext = {
  route: Routing.Route.t,
  authToken: option(string),
  loggedUser: option(Types.user),
  globalSetting: option(Types.globalSetting),
};

module AppContext =
  Contexts.MakePair({
    type t = appContext;
    let defaultValue = {
      route: Home,
      authToken: None,
      loggedUser: None,
      globalSetting: None,
    };
  });
