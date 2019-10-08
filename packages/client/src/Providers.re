type appContext = {
  loggedInUser: option(Types.user),
  globalSetting: option(Types.globalSetting),
};

module AppContext =
  Contexts.MakePair({
    type t = appContext;
    let defaultValue = {loggedInUser: None, globalSetting: None};
  });
