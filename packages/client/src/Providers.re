type appContext = {authToken: option(string)};

module AppContext =
  Contexts.MakePair({
    type t = appContext;
    let defaultValue = {authToken: None};
  });