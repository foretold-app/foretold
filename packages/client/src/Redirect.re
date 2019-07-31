let component = ReasonReact.statelessComponent("Redirect");

let countOfRedicrectings = ref(0);

// @todo: fix this duplicated ligic(#rederectings)
let make = (~appContext: Providers.appContext, _children) => {
  ...component,
  didUpdate: _self => {
    countOfRedicrectings := countOfRedicrectings^ + 1;

    switch (appContext.loggedInUser, appContext.route, countOfRedicrectings^) {
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