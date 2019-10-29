[@bs.config {jsx: 3}];

module E = FC__E;

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

[@react.component]
let make =
    (
      ~initialValue,
      ~values: list(('a, string)),
      ~onSelect: option(option('a) => unit)=?,
      ~trigger=FC__Dropdown.Click,
    ) => {
  let initLabel = "";

  let (label, setLabel) = React.useState(() => initLabel);

  <FC__DropdownMenu title=label trigger>
    <FC__Menu
      selectable=true
      onSelect={(info: FC__Menu.clickInfo) =>
        switch (
          (values |> E.L.withIdx)
          ->(E.L.getBy(((i, _)) => info.key == "key" ++ string_of_int(i)))
        ) {
        | Some((_i, (key, label))) =>
          setLabel(_ => label);
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
};

module Jsx2 = {
  let make =
      (
        ~initialValue: option('a)=None,
        ~values: list(('a, string)),
        ~onSelect: option(option('a) => unit)=?,
        ~trigger=FC__Dropdown.Click,
        children,
      ) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~initialValue, ~values, ~onSelect?, ~trigger, ()),
      children,
    );
};