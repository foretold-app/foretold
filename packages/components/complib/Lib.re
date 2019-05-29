open FC.Base;

type compEntry = {
  id: string,
  title: string,
  render: unit => React.element,
};

let entries: list(compEntry) = [
  {
    id: "l1",
    title: "Link1",
    render: () => <Link> "Test link"->React.string </Link>,
  },
  {
    id: "l2",
    title: "Link2",
    render: () => <Link> "Test link2"->React.string </Link>,
  },
  {
    id: "l3",
    title: "Link3",
    render: () => <Link> "Test link3"->React.string </Link>,
  },
];

let baseUrl = "/complib/complib.html";

module Index = {
  type state = {route: ReasonReactRouter.url};

  type action =
    | ItemClick(compEntry)
    | ChangeRoute(ReasonReactRouter.url);

  let reducer = (action: action, _state: state) => {
    switch (action) {
    | ItemClick(e) =>
      ReasonReact.SideEffects(
        _ => ReasonReactRouter.push(baseUrl ++ "#" ++ e.id),
      )
    | ChangeRoute(route) => ReasonReact.Update({route: route})
    };
  };

  let component = ReasonReact.reducerComponent(__MODULE__);
  let make = _children => {
    ...component,
    initialState: () => {
      /* Not the correct url at this point */
      route: {
        path: [],
        hash: "",
        search: "",
      },
    },
    reducer,
    didMount: self => {
      let watcherID =
        ReasonReactRouter.watchUrl(url => self.send(ChangeRoute(url)));
      self.onUnmount(() => ReasonReactRouter.unwatchUrl(watcherID));
    },
    render: self =>
      <div>
        {(
           entries
           |> E.L.fmap(e =>
                <div key={e.id} onClick={_e => self.send(ItemClick(e))}>
                  e.title->React.string
                </div>
              )
           |> E.L.toArray
         )
         ->React.array}
        {switch (E.L.getBy(entries, e => e.id == self.state.route.hash)) {
         | Some(e) => e.render()
         | None => React.null
         }}
      </div>,
  };
};
