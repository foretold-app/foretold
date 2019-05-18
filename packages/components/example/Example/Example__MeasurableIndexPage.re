open FC;
open Base;

let row =
  <Table.Row>
    <Table.Cell
      flex=6
      styles=[
        Css.(style([paddingTop(`em(1.0)), paddingBottom(`em(0.5))])),
      ]>
      <div className=Table.Row.headerStyle>
        {"What will the GDP of China be in 2020?" |> ReasonReact.string}
      </div>
      <div
        className=Css.(
          style([
            Colors.FontWeights.heavy,
            color(Colors.Statuses.green),
            fontSize(`em(0.9)),
          ])
        )>
        {"Open" |> ReasonReact.string}
      </div>
    </Table.Cell>
    <Table.Cell
      flex=2
      styles=[
        Css.(style([paddingTop(`em(1.0)), paddingBottom(`em(0.5))])),
      ]>
      <FC__CdfChart__Small
        data={"xs": ExampleCdfs.Example1.xs, "ys": ExampleCdfs.Example1.ys}
        minX=2.0
        color={`hex("#d9dcdf")}
        maxX=12.0
      />
    </Table.Cell>
    <Table.Cell flex=1 styles=[Css.(style([paddingTop(`em(0.5))]))]>
      <Div>
        <Link
          styles=[Css.marginRight(`em(1.0))]
          colors=(Colors.textMedium, Colors.textDark)>
          {"Series A" |> ReasonReact.string}
        </Link>
        <Link colors=(Colors.textMedium, Colors.textDark)>
          {"19" |> ReasonReact.string}
        </Link>
      </Div>
      <Div>
        <Link
          colors=(Colors.textMedium, Colors.textDark)
          styles=[
            Css.marginRight(`em(1.0)),
            Css.textDecoration(`underline),
          ]>
          {"Edit" |> ReasonReact.string}
        </Link>
        <Link
          styles=[Css.textDecoration(`underline)]
          colors=(Colors.textMedium, Colors.textDark)>
          {"Archive" |> ReasonReact.string}
        </Link>
      </Div>
    </Table.Cell>
  </Table.Row>;

let make =
  <PageCard>
    {
      PageCard.header(
        <Div>
          <Div>
            {
              Simple.activeItemWithNumber(
                true,
                "Open" |> ReasonReact.string,
                12,
              )
            }
            {
              Simple.activeItemWithNumber(
                false,
                "Pending Resolution" |> ReasonReact.string,
                18,
              )
            }
            {
              Simple.activeItemWithNumber(
                false,
                "Closed" |> ReasonReact.string,
                10,
              )
            }
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
    <Table.HeaderRow>
      <Table.Cell
        flex=6
        styles=[
          Css.(style([paddingTop(`em(0.7)), paddingBottom(`em(0.7))])),
        ]>
        {"Name & Status" |> ReasonReact.string}
      </Table.Cell>
      <Table.Cell
        flex=2
        styles=[
          Css.(style([paddingTop(`em(0.7)), paddingBottom(`em(0.7))])),
        ]>
        {"Aggregate Prediction" |> ReasonReact.string}
      </Table.Cell>
      <Table.Cell
        flex=1
        styles=[
          Css.(style([paddingTop(`em(0.7)), paddingBottom(`em(0.7))])),
        ]>
        {"Details" |> ReasonReact.string}
      </Table.Cell>
    </Table.HeaderRow>
    row
    row
    row
    row
    row
    row
    row
    row
    row
  </PageCard>;