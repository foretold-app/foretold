open FC;
open FC.Base;

let colors =
  Colors.[
    ("white", white),
    ("black", black),
    ("greyO4", greyO4),
    ("whiteO2", whiteO2),
    ("whiteOc", whiteOc),
    ("clear", clear),
    ("textDarker", textDarker),
    ("textDark", textDark),
    ("textMedium", textMedium),
    ("smokeWhite", smokeWhite),
    ("buttonHover", buttonHover),
    ("lightGrayBackground", lightGrayBackground),
    ("lighterGrayBackground", lighterGrayBackground),
    ("grayBackground", grayBackground),
    ("greydisabled", greydisabled),
    ("accentBlue", accentBlue),
    ("accentBlueO8", accentBlueO8),
    ("accentBlue1a", accentBlue1a),
    ("mainBlue", mainBlue),
    ("link", link),
    ("linkHover", linkHover),
    ("linkAccent", linkAccent),
    ("darkLink", darkLink),
    ("darkAccentBlue", darkAccentBlue),
    ("grey1", grey1),
    ("border", border),
    ("primary", primary),
  ];

let colorBoxStyle =
  Css.(
    style([
      flexBasis(`px(220)),
      flexGrow(1.),
      height(`px(90)),
      marginRight(`px(35)),
      marginBottom(`px(35)),
      textAlign(`center),
    ])
  );

let colorContainer = bgColor =>
  Css.(
    style([
      display(`flex),
      flexWrap(`wrap),
      padding(`em(1.5)),
      backgroundColor(bgColor),
    ])
  );

module ColorDisplay = {
  type state = {
    bgColor: Css.color,
    bgName: string,
  };
  type action =
    | ChangeBg(string);
  let reducer = (action: action, _state: state) => {
    switch (action) {
    | ChangeBg(bgName) =>
      let (_, bgColor) = colors |> E.L.find(((n, _)) => n == bgName);
      ReasonReact.Update({bgColor, bgName});
    };
  };
  let component = ReasonReact.reducerComponent(__MODULE__);
  let make = _children => {
    ...component,
    initialState: () => {bgColor: Colors.white, bgName: "white"},
    reducer,
    render: self =>
      <PageCard>
        <PageCard.HeaderRow>
          <PageCard.HeaderRow.Title>
            "Colors"->React.string
          </PageCard.HeaderRow.Title>
        </PageCard.HeaderRow>
        <PageCard.Section borderBottom=true>
          "Background: "->React.string
          <select
            value={self.state.bgName}
            onChange={e =>
              self.send(ChangeBg(ReactEvent.Form.target(e)##value))
            }>
            {{colors
              |> E.L.fmap(((name, _c)) =>
                   <option key=name value=name> name->React.string </option>
                 )
              |> E.L.toArray}
             ->React.array}
          </select>
        </PageCard.Section>
        <div className={colorContainer(self.state.bgColor)}>
          {{colors
            |> E.L.fmap(((name, c)) =>
                 <div
                   key=name
                   className=Css.(
                     merge([colorBoxStyle, style([backgroundColor(c)])])
                   )>
                   name->React.string
                 </div>
               )
            |> E.L.toArray}
           ->React.array}
        </div>
      </PageCard>,
  };
};

let entry =
  EntryTypes.(entry(~title="Colors", ~render=() => <ColorDisplay />));
