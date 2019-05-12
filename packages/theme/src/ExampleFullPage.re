let make =
  <Div>
    Example__AppGroupHeader.make
    <Div
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
            @ Base.fullWidthFloatLeft,
          )
        ),
      ]>
      Example__MeasurableIndexPage.make
    </Div>
    <Div
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
            @ Base.fullWidthFloatLeft,
          )
        ),
      ]>
      <Div flexDirection=`row>
        <Div flex=5 styles=[Css.(style([paddingRight(`em(2.0))]))]>
          <Div> Example__MeasurableTopCard.make </Div>
          <Div
            styles=[
              Css.(style([marginTop(`em(2.0))] @ Base.fullWidthFloatLeft)),
            ]>
            Example__CardMeasurableMeasurements.make
          </Div>
        </Div>
        <Div flex=2> Example__MeasurableTopCard.make </Div>
      </Div>
    </Div>
  </Div>;