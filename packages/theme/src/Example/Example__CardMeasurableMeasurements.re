let make =
  <PageCard>
    {
      PageCard.header(
        <Div>
          <Div
            styles=[
              Css.style([
                Base.floatLeft,
                Css.padding3(~top=`em(0.2), ~bottom=`em(0.0), ~h=`em(1.5)),
              ]),
            ]>
            <Tab isActive=true> {"Trends" |> ReasonReact.string} </Tab>
            <Tab isActive=false> {"Predictions" |> ReasonReact.string} </Tab>
            <Tab isActive=false> {"Settings" |> ReasonReact.string} </Tab>
          </Div>
        </Div>,
      )
    }
    <Div
      styles=[
        Css.style(
          [Css.padding2(~v=`em(1.5), ~h=`em(1.5))]
          @ Base.fullWidthFloatLeft,
        ),
      ]>
      <PageCard.P>
        {
          "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut vulputate tortor a sapien aliquet ullamcorper. Nunc non varius sapien, quis elementum sapien. Morbi ac tristique quam. Cras hendrerit accumsan pretium. Praesent id nisl sit amet eros imperdiet placerat. Vestibulum sodales posuere diam vel laoreet."
          |> ReasonReact.string
        }
      </PageCard.P>
    </Div>
  </PageCard>;