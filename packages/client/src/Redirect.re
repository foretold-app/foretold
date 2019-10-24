[@bs.config {jsx: 3}];

// This var need to prevent when "a user cannot go from profile page".
let redirectionCount = ref(0);

//@todo: 1
[@react.component]
let make = (~appContext: Providers.appContext) => {
  redirectionCount := redirectionCount^ + 1;

  switch (appContext.loggedUser, appContext.route, redirectionCount^) {
  | (_, Profile, _) => ()
  | (Some(loggedUser), _, 2) =>
    loggedUser.agent
    |> E.O.fmap((agent: Types.agent) =>
         switch (agent.name) {
         | Some("") =>
           Routing.Url.push(Profile);
           Antd.Message.info(Lang.nameIsEmpty |> Utils.ste, 30) |> ignore;
           ();
         | _ => ()
         }
       )
    |> E.O.default()
  | _ => ()
  };

  ReasonReact.null;
};
