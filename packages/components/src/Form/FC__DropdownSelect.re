[@bs.config {jsx: 3}];

module E = FC__E;

type state = {label: string};

type action =
  | ChangeLabel(string);

/**
   * Dropdown select
   * Usage
   * <DropdownSelect.String
   *   selected=Some("key2")
   *   values=[("key1", "Label1"), ("key2", "Label2")]
   *   onSelect={v =>
   *     switch (v) {
   *       | Some(k) => Js.log2("Selected: ", k)
   *       | None => Js.log("Selected none")
   *     }
   *   }
   * />
   *
   */
let component = ReasonReact.reducerComponent(__MODULE__);
let make =
    (
      ~initialValue: option('a)=None,
      ~values: list(('a, string)),
      ~onSelect: option(option('a) => unit)=?,
      ~trigger=FC__Dropdown.Click,
      _children,
    ) => {
  ...component,
  initialState: () => {label: ""},
  reducer: (action: action, _state: state) => {
    switch (action) {
    | ChangeLabel(label) => ReasonReact.Update({label: label})
    };
  },
  didMount: self => {
    switch (initialValue) {
    | None => ()
    | Some(initKey) =>
      // Try to get pair of (key, value) from values to use as
      // initial value and label
      switch (E.L.getBy(values, ((key, _)) => initKey == key)) {
      | Some((_, label)) => self.send(ChangeLabel(label))
      | None => () // Error intial value pair not found
      }
    };
  },
  render: self => {
    <FC__DropdownMenu title={self.state.label} trigger>
      <FC__Menu
        selectable=true
        onSelect={(info: FC__Menu.clickInfo) =>
          switch (
            (values |> E.L.withIdx)
            ->(E.L.getBy(((i, _)) => info.key == "key" ++ string_of_int(i)))
          ) {
          | Some((_i, (key, label))) =>
            self.send(ChangeLabel(label));
            switch (onSelect) {
            | Some(onSelect) => onSelect(Some(key))
            | None => ()
            };
          | None => () // Error, could not find selected key among values
          }
        }>
        // Was a little worried about using a straight int as a key,
        // in case some conversions may go wrong or something, so added
        // "key" before

          {values
           |> E.L.React.fmapi((i, (_key, label)) =>
                <FC__Menu.Item key={"key" ++ string_of_int(i)}>
                  label->React.string
                </FC__Menu.Item>
              )}
        </FC__Menu>
    </FC__DropdownMenu>;
  },
};
