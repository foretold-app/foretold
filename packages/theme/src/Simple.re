let tagStyles =
  Css.[
    padding2(~v=`px(2), ~h=`px(14)),
    Base.floatLeft,
    borderRadius(`px(7)),
    border(`px(1), `solid, `hex("D5D7DAAA")),
  ];

let aStyles =
  Css.[Base.floatLeft, marginRight(`em(1.0)), marginTop(`px(3))];

let tag = (text, number: int) =>
  <BaseLink
    colors=(Colors.darkAccentBlue, Colors.grey1)
    styles=Css.[Base.floatLeft, padding2(~v=`em(1.), ~h=`em(1.5))]>
    <span className={Css.style(aStyles)}> text </span>
    <span className={Css.style(tagStyles)}>
      {number |> string_of_int |> ReasonReact.string}
    </span>
  </BaseLink>;