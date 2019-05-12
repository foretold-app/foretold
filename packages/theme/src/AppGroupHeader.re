let make =
  <Div>
    <Div>
      <GroupHeader hero={"US Politics" |> ReasonReact.string}>
        <Div float=`left>
          <div
            className=Css.(
              style([
                fontSize(`px(24)),
                fontWeight(`num(500)),
                color(GroupHeader.Colors'.primary),
              ])
            )>
            {"Great Group" |> ReasonReact.string}
          </div>
          <p
            className=Css.(
              style([
                color(GroupHeader.Colors'.secondary),
                marginTop(`px(0)),
              ])
            )>
            {
              "Lots of description for the group would be here"
              |> ReasonReact.string
            }
          </p>
        </Div>
        <Div float=`right>
          {
            GroupHeader.secondaryLink(
              ~href="#",
              [|"Leave Channel" |> ReasonReact.string|],
            )
            |> ReasonReact.element
          }
          {
            GroupHeader.link(
              ~href="#",
              [|"Create Question" |> ReasonReact.string|],
            )
            |> ReasonReact.element
          }
        </Div>
      </GroupHeader>
      <GroupHeader.SubHeader>
        {
          GroupHeader.SubHeader.linkItem(
            ~isActive=true,
            [|"Questions" |> ReasonReact.string|],
          )
        }
        {
          GroupHeader.SubHeader.linkItem([|
            "Knowledge Graph" |> ReasonReact.string,
          |])
        }
        {
          GroupHeader.SubHeader.linkItem([|
            "Leaderboard" |> ReasonReact.string,
          |])
        }
        {GroupHeader.SubHeader.linkItem([|"Settings" |> ReasonReact.string|])}
      </GroupHeader.SubHeader>
    </Div>
    <Div
      styles=[
        Css.(
          style(
            [
              marginTop(`em(2.)),
              float(`left),
              width(`percent(100.)),
              paddingLeft(`em(2.)),
              paddingRight(`em(2.)),
              boxSizing(`borderBox),
            ]
            @ Base.fullWidthFloatLeft,
          )
        ),
      ]>
      <PageCard>
        {
          PageCard.header(
            [|
              Simple.activeItemWithNumber("Open" |> ReasonReact.string, 12),
              Simple.activeItemWithNumber(
                "Pending Resolution" |> ReasonReact.string,
                18,
              ),
              Simple.activeItemWithNumber("Closed" |> ReasonReact.string, 10),
              PaginationButtons.make({
                currentValue: Item(3),
                max: 100,
                pageLeft: {
                  isDisabled: false,
                  onClick: _ => (),
                },
                pageRight: {
                  isDisabled: true,
                  onClick: _ => (),
                },
              }),
            |]
            |> ReasonReact.array,
          )
        }
        <Table.HeaderRow>
          <Table.Cell
            flex=2
            styles=[
              Css.(
                style([paddingTop(`em(0.7)), paddingBottom(`em(0.7))])
              ),
            ]>
            {"Name & Status" |> ReasonReact.string}
          </Table.Cell>
          <Table.Cell
            flex=1
            styles=[
              Css.(
                style([paddingTop(`em(0.7)), paddingBottom(`em(0.7))])
              ),
            ]>
            {"Prediction Over Time" |> ReasonReact.string}
          </Table.Cell>
          <Table.Cell
            flex=1
            styles=[
              Css.(
                style([paddingTop(`em(0.7)), paddingBottom(`em(0.7))])
              ),
            ]>
            {"Details" |> ReasonReact.string}
          </Table.Cell>
        </Table.HeaderRow>
        <Table.Row>
          <Table.Cell
            flex=2
            styles=[
              Css.(
                style([paddingTop(`em(1.0)), paddingBottom(`em(0.5))])
              ),
            ]>
            <div className=Table.Row.headerStyle>
              {"What will the GDP of China be in 2020?" |> ReasonReact.string}
            </div>
            <div
              className=Css.(
                style([
                  fontWeight(`num(500)),
                  color(Colors.Statuses.green),
                ])
              )>
              {"Open" |> ReasonReact.string}
            </div>
          </Table.Cell>
          <Table.Cell flex=1 styles=[Css.(style([paddingTop(`em(1.0))]))]>
            {"Knowledge Graph" |> ReasonReact.string}
          </Table.Cell>
          <Table.Cell flex=1 styles=[Css.(style([paddingTop(`em(1.0))]))]>
            {"Knowledge Graph" |> ReasonReact.string}
          </Table.Cell>
        </Table.Row>
      </PageCard>
    </Div>
  </Div>;