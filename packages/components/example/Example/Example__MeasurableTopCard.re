open FC;
open Base;

let pastTime = 1483232400;
let futureTime = 1559005200;

let make =
  <PageCard>
    <PageCard.HeaderRow>
      <Div>
        <Div
          styles=[
            Css.style([
              BaseStyles.floatLeft,
              PageCard.HeaderRow.Styles.itemTopPadding,
              PageCard.HeaderRow.Styles.itemBottomPadding,
            ]),
          ]>
          <FC__Button> {"< Back" |> ReasonReact.string} </FC__Button>
        </Div>
        <Div
          float=`right
          styles=[
            Css.style([
              PageCard.HeaderRow.Styles.itemTopPadding,
              PageCard.HeaderRow.Styles.itemBottomPadding,
            ]),
          ]>
          {PaginationButtons.make({
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
           })}
        </Div>
      </Div>
    </PageCard.HeaderRow>
    <Div
      styles=[
        Css.style(
          [Css.padding2(~v=`em(1.5), ~h=`em(1.5))]
          @ BaseStyles.fullWidthFloatLeft,
        ),
      ]>
      <Div flexDirection=`row>
        <Div flex={`num(5.)}>
          <PageCard.H1>
            {"What would the US GDP Be in 2020?" |> ReasonReact.string}
          </PageCard.H1>
          {FC__StateStatus.make(
             ~state=RESOLVED(MomentRe.momentWithUnix(pastTime)),
             ~fontSize=`em(1.0),
             (),
           )}
        </Div>
        <Div flex={`num(1.)}>
          <FC__AgentLink>
            {"Roger Adams" |> ReasonReact.string}
          </FC__AgentLink>
        </Div>
      </Div>
      <Div styles=[Css.style([Css.marginTop(`em(2.0))])]>
        <PageCard.P>
          {"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut vulputate tortor a sapien aliquet ullamcorper. Nunc non varius sapien, quis elementum sapien. Morbi ac tristique quam. Cras hendrerit accumsan pretium. Praesent id nisl sit amet eros imperdiet placerat. Vestibulum sodales posuere diam vel laoreet."
           |> ReasonReact.string}
        </PageCard.P>
      </Div>
    </Div>
  </PageCard>;