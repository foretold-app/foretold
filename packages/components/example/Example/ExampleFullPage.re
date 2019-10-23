open FC__Base;
open FC;
let make =
  <Div.Jsx2>
    <AppHeader.Jsx2
      links={
        [|
          AppHeader.link(
            ~href="#",
            [|"Public Groups" |> ReasonReact.string|],
          )
          |> ReasonReact.element,
          AppHeader.link(
            ~href="#",
            [|"Entity Explorer" |> ReasonReact.string|],
          )
          |> ReasonReact.element,
        |]
        |> ReasonReact.array
      }
    />
    Example__AppGroupHeader.make
    <Div.Jsx2
      styles=[
        Css.(
          style(
            [
              marginTop(`em(1.)),
              width(`percent(100.)),
              paddingLeft(`em(2.)),
              paddingRight(`em(2.)),
            ]
            @ BaseStyles.fullWidthFloatLeft,
          )
        ),
      ]>
      Example__MeasurableIndexPage.make
    </Div.Jsx2>
    <Div.Jsx2
      styles=[
        Css.(
          style(
            [
              marginTop(`em(1.)),
              width(`percent(100.)),
              paddingLeft(`em(2.)),
              paddingRight(`em(2.)),
              boxSizing(`borderBox),
            ]
            @ BaseStyles.fullWidthFloatLeft,
          )
        ),
      ]>
      <Div.Jsx2 flexDirection=`row>
        <Div.Jsx2
          flex={`num(5.0)} styles=[Css.(style([paddingRight(`em(2.0))]))]>
          <Div.Jsx2> Example__MeasurableTopCard.make </Div.Jsx2>
          <Div.Jsx2
            styles=[
              Css.(
                style(
                  [marginTop(`em(1.0))] @ BaseStyles.fullWidthFloatLeft,
                )
              ),
            ]>
            Example__CardMeasurableMeasurements.make
          </Div.Jsx2>
        </Div.Jsx2>
        <Div.Jsx2 flex={`num(2.0)}>
          Example__MeasurableTopCard.make
          <Div.Jsx2
            styles=[Css.(style([clear(`both), paddingTop(`em(1.0))]))]>
            <MeasurableForm cdf=ExampleCdfs.Example1.cdf />
          </Div.Jsx2>
        </Div.Jsx2>
      </Div.Jsx2>
    </Div.Jsx2>
    <Div.Jsx2
      styles=[
        Css.(
          style(
            [
              marginTop(`em(2.)),
              width(`percent(100.)),
              paddingLeft(`em(2.)),
              paddingRight(`em(2.)),
              boxSizing(`borderBox),
            ]
            @ BaseStyles.fullWidthFloatLeft,
          )
        ),
      ]>
      <Div.Jsx2 flexDirection=`row>
        <Div.Jsx2
          flex={`num(5.0)} styles=[Css.(style([paddingRight(`em(2.0))]))]>
          <Div.Jsx2> Example__MemberTableCard.make </Div.Jsx2>
        </Div.Jsx2>
        <Div.Jsx2 flex={`num(2.0)} />
      </Div.Jsx2>
    </Div.Jsx2>
    <Footer.Jsx2
      logo={React.string({js|2019 \u00a9 Foretold|js})}
      links=[|
        <a href="#"> {React.string("About")} </a>,
        <a href="#"> {React.string("Help")} </a>,
        <a href="#"> {React.string("Documentation")} </a>,
        <a href="#"> {React.string("Privacy Policy")} </a>,
        <a href="#"> {React.string("Terms of Service")} </a>,
      |]
    />
  </Div.Jsx2>;
