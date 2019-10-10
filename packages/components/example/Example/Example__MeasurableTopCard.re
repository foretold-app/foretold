open FC;
open Base;

let pastTime = 1483232400;
let futureTime = 1559005200;

let make =
  <PageCard>
    <PageCard.HeaderRow>
      <Div.Jsx2>
        <Div.Jsx2
          styles=[
            Css.style([
              BaseStyles.floatLeft,
              PageCard.HeaderRow.Styles.itemTopPadding,
              PageCard.HeaderRow.Styles.itemBottomPadding,
            ]),
          ]>
          <FC__Button size=Small>
            {"< Back" |> ReasonReact.string}
          </FC__Button>
        </Div.Jsx2>
        <Div.Jsx2
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
        </Div.Jsx2>
      </Div.Jsx2>
    </PageCard.HeaderRow>
    <Div.Jsx2
      styles=[
        Css.style(
          [Css.padding2(~v=`em(1.5), ~h=`em(1.5))]
          @ BaseStyles.fullWidthFloatLeft,
        ),
      ]>
      <Div.Jsx2 flexDirection=`row>
        <Div.Jsx2 flex={`num(5.)}>
          <PageCard.H1>
            {"What would the US GDP Be in 2020?" |> ReasonReact.string}
          </PageCard.H1>
          {FC__StateStatus.make(
             ~state=RESOLVED(MomentRe.momentWithUnix(pastTime)),
             ~fontSize=`em(1.0),
             (),
           )}
        </Div.Jsx2>
        <FC__AgentLink.Jsx2
          agent={FC__AgentLink.Agent.makeUser(
            ~name="Roger Adams",
            ~image=
              "https://lh3.googleusercontent.com/-1sj3EqkojJ4/AAAAAAAAAAI/AAAAAAAAAAA/ACHi3rfWCVqnuJxxM41Zird4HZx0BbRpbQ/photo.jpg",
            (),
          )}
        />
      </Div.Jsx2>
      <Div.Jsx2 styles=[Css.style([Css.marginTop(`em(2.0))])]>
        <PageCard.P>
          {"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut vulputate tortor a sapien aliquet ullamcorper. Nunc non varius sapien, quis elementum sapien. Morbi ac tristique quam. Cras hendrerit accumsan pretium. Praesent id nisl sit amet eros imperdiet placerat. Vestibulum sodales posuere diam vel laoreet."
           |> ReasonReact.string}
        </PageCard.P>
      </Div.Jsx2>
    </Div.Jsx2>
  </PageCard>;