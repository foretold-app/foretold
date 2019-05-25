let load = () => {
  Css.(
    global(
      "body",
      [
        fontFamily("Lato"),
        margin(`zero),
        height(`percent(100.0)),
        background(`hex("F0F1F3")),
      ],
    )
  );

  Css.(global("html", [height(`percent(100.0)), fontSize(`px(16))]));
};