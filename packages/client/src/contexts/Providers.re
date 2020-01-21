type appContext = {
  loggedUser: option(Types.user),
  globalSetting: option(Types.globalSetting),
};

let appContextInit: appContext = {loggedUser: None, globalSetting: None};
let app = React.createContext(appContextInit);

module AppContexProvider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
  let make = React.Context.provider(app);
};
