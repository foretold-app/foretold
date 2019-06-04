// Helper to find by "key1", "key2" etc
// where the number is the index in the list.
// Was a little worried about using straight ints
// as strings (if they would unintentionally be
// converted to ints in some situations)
// so prepended "key"
let findByIndexKey = (key, values) => {
  let rec loop = (i, rest) =>
    switch (rest) {
    | [first, ...rest] =>
      if ("key" ++ string_of_int(i) == key) {
        Some(first);
      } else {
        loop(i + 1, rest);
      }
    | [] => None
    };
  loop(0, values);
};
module type Template = {type keyType;};
module Make = (T: Template) => {
  type state = {
    value: option(T.keyType),
    label: string,
    isInitialized: bool,
  };

  type action =
    | ChangeValue(option(T.keyType), string)
    | Initialize(option(T.keyType), string);

  let component = ReasonReact.reducerComponent(__MODULE__);
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
   * You can make select with key of your own type with:
   *
   * module SelectMyType = DropdownSelect.Make({
   *   type keyType = myType
   * });
   *
   */
  let make =
      (
        ~initialValue: option(T.keyType)=None,
        ~values: list((T.keyType, string)),
        ~onSelect: option(T.keyType) => unit,
        _children,
      ) => {
    ...component,
    initialState: () => {value: None, label: "", isInitialized: false},
    reducer: (action: action, state: state) => {
      switch (action) {
      | ChangeValue(value, label) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, value, label},
          _self => onSelect(value),
        )
      | Initialize(value, label) =>
        ReasonReact.Update({value, label, isInitialized: true})
      };
    },
    didMount: self => {
      switch (initialValue) {
      | None => self.send(Initialize(None, ""))
      | Some(initKey) =>
        // Try to get pair of (key, value) from values to use as
        // initial value and label
        switch (FC__E.L.getBy(values, ((key, _)) => initKey == key)) {
        | Some((value, label)) => self.send(Initialize(Some(value), label))
        | None => () // Error intial value pair not found
        }
      };
    },
    render: self => {
      <FC__DropdownMenu title={self.state.label}>
        <FC__Menu
          selectable=true
          onSelect={info =>
            switch (findByIndexKey(info.key, values)) {
            | Some((key, label)) =>
              self.send(ChangeValue(Some(key), label))
            | None => () // Error, could not find selected key among values
            }
          }>
          {values
           |> FC__E.L.React.mapi((i, (_key, label)) =>
                <FC__Menu.Item key={"key" ++ string_of_int(i)}>
                  label->React.string
                </FC__Menu.Item>
              )}
        </FC__Menu>
      </FC__DropdownMenu>;
    },
  };
};

module String =
  Make({
    type keyType = string;
  });

module Int =
  Make({
    type keyType = int;
  });
