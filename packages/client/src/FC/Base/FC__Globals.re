let commentStyle =
  Css.[
    padding3(~top=`em(0.4), ~bottom=`em(0.3), ~h=`em(1.1)),
    background(FC__Colors.lightGrayBackground),
    borderRadius(FC__Colors.BorderRadius.tight),
    color(FC__Colors.Text.LightBackground.p),
  ];

let load = () => {
  Css.(
    global(
      "body",
      [
        fontFamily(FC__Colors.Text.standardFont),
        margin(`zero),
        height(`percent(100.0)),
        background(`hex("F0F1F3")),
        fontSize(`px(16)),
        selector("a", [color(`hex("1256ab"))]),
        selector("blockquote", commentStyle),
        selector(
          "hr",
          [
            marginTop(`em(1.)),
            marginBottom(`em(1.)),
            borderLeft(`px(0), `none, `hex("000")),
            borderRight(`px(0), `solid, `hex("000")),
            borderBottom(`px(0), `none, `hex("000")),
            borderTop(`px(1), `solid, `hex("f0f2f5")),
          ],
        ),
      ],
    )
  );

  Css.(global("html", [height(`percent(100.0))]));
};