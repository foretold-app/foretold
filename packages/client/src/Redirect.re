// This var need to prevent when "a user cannot go from profile page".
let redirectionCount = ref(0);

let component = ReasonReact.statelessComponent("Redirect");
let make = (~appContext: Providers.appContext, _children) => {
  ...component,
  didUpdate: _self => {
    redirectionCount := redirectionCount^ + 1;

    switch (appContext.loggedUser, appContext.route, redirectionCount^) {
    | (_, Profile, _) => ()
    | (Some(loggedUser), _, 2) =>
      loggedUser.agent
      |> E.O.fmap((agent: Types.agent) =>
           switch (agent.name) {
           | Some("") =>
             Routing.Url.push(Profile);
             Antd.Message.info(
               ~content=Lang.nameIsEmpty |> Utils.ste,
               ~duration=30.,
               (),
             )
             |> ignore;
             ();
           | _ => ()
           }
         )
      |> E.O.default()
    | _ => ()
    };
  },

  render: _ => ReasonReact.null,
};
