let row =
  <Table.Row>
    <Table.Cell
      flex=6
      styles=[
        Css.(style([paddingTop(`em(0.8)), paddingBottom(`em(0.3))])),
      ]>
      <div className=Table.Row.headerStyle>
        {"What will the GDP of China be in 2020?" |> ReasonReact.string}
      </div>
      <div
        className=Css.(
          style([fontWeight(`num(500)), color(Colors.Statuses.green)])
        )>
        {"Open" |> ReasonReact.string}
      </div>
    </Table.Cell>
    <Table.Cell flex=1 styles=[Css.(style([paddingTop(`em(0.5))]))]>
      <Div>
        <BaseLink styles=[Css.marginRight(`em(1.0))]>
          {"Series A" |> ReasonReact.string}
        </BaseLink>
        <BaseLink> {"19" |> ReasonReact.string} </BaseLink>
      </Div>
      <Div>
        <BaseLink
          styles=[
            Css.marginRight(`em(1.0)),
            Css.textDecoration(`underline),
          ]>
          {"Edit" |> ReasonReact.string}
        </BaseLink>
        <BaseLink styles=[Css.textDecoration(`underline)]>
          {"Archive" |> ReasonReact.string}
        </BaseLink>
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