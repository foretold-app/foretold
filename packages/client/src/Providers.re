type appContext = {
  loggedUser: option(Types.user),
  globalSetting: option(Types.globalSetting),
};

module AppContext =
  Contexts.MakePair({
    type t = appContext;
    let defaultValue = {loggedUser: None, globalSetting: None};
  });