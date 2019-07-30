let component = ReasonReact.statelessComponent("Redirect");

// @todo: fix this duplicated ligic(#rederectings)
let make = (~appContext: Providers.appContext, _children) => {
  ...component,
  didUpdate: self => {
    Js.log2("Rounting Count:", appContext.routingCount);

    switch (
      appContext.loggedInUser,
      appContext.route,
      appContext.routingCount,
    ) {
    | (_, Profile, _) => ()
    | (Some(loggedInUser), _, 1) =>
      loggedInUser.agent
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