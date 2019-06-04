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
        ~onSelect: option(option(T.keyType) => unit)=?,
        ~trigger=FC__Dropdown.Click,
        _children,
      ) => {
    ...component,
    initialState: () => {value: None, label: "", isInitialized: false},
    reducer: (action: action, state: state) => {
      switch (action) {
      | ChangeValue(value, label) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, value, label},
          _self => switch (onSelect) {
            | Some(onSelect) => onSelect(value)
            | None => ()
          }
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
      <FC__DropdownMenu title={self.state.label} trigger>
        <FC__Menu
          selectable=true
          onSelect={info =>
            switch (
              FC__E.L.findWithIndex(
                (i, _) => info.key == "key" ++ string_of_int(i),
                values,
              )
            ) {
            | Some((key, label)) =>
              self.send(ChangeValue(Some(key), label))
            | None => () // Error, could not find selected key among values
            }
          }>
          // Was a little worried about using a straight int as a key,
          // in case some conversions may go wrong or something, so added
          // "key" before
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
