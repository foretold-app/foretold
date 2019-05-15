open FC;
open Base;

let make =
  <PageCard>
    {
      PageCard.header(
        <Div>
          <Div
            styles=[
              Css.style([
                BaseStyles.floatLeft,
                Css.padding2(~v=`em(0.6), ~h=`em(1.5)),
              ]),
            ]>
            <Simple.Linktag> {"< Back" |> ReasonReact.string} </Simple.Linktag>
          </Div>
          <Div float=`right>
            {
              PaginationButtons.make({
                currentValue: Range(3, 10),
                max: 100,
                pageLeft: {
                  isDisabled: false,
                  onClick: _ => (),
                },
                pageRight: {
                  isDisabled: true,
                  onClick: _ => (),
                },
              })
            }
          </Div>
        </Div>,
      )
    }
    <Div
      styles=[
        Css.style(
          [Css.padding2(~v=`em(1.5), ~h=`em(1.5))]
          @ BaseStyles.fullWidthFloatLeft,
        ),
      ]>
      <Div flexDirection=`row>
        <Div flex=5>
          <PageCard.H1>
            {"What would the US GDP Be in 2020?" |> ReasonReact.string}
          </PageCard.H1>
        </Div>
        <Div flex=1>
          {"What would the US GDP Be in 2020?" |> ReasonReact.string}
        </Div>
      </Div>
      <PageCard.P>
        {
          "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut vulputate tortor a sapien aliquet ullamcorper. Nunc non varius sapien, quis elementum sapien. Morbi ac tristique quam. Cras hendrerit accumsan pretium. Praesent id nisl sit amet eros imperdiet placerat. Vestibulum sodales posuere diam vel laoreet."
          |> ReasonReact.string
        }
      </PageCard.P>
    </Div>
  </PageCard>;