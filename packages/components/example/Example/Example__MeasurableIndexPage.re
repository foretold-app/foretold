open FC;
open Base;

let cdf = ExampleCdfs.Example1.cdf;

let futureTime = 1559005200;

let foo = MomentRe.momentNow();

let llink =
  FC__Link.make(
    ~isDisabled=false,
    ~className=
      Css.(
        style([
          textDecoration(`underline),
          color(`hex("384e67")),
          hover([color(Colors.link)]),
        ])
      ),
  );

let row =
  <Table.RowLink onClick={_ => Js.log("Row Clicked")}>
    <Table.Cell
      flex=4
      styles=[
        Css.(style([paddingTop(`em(1.0)), paddingBottom(`em(0.5))])),
      ]>
      <div>
        <span className=Table.Row.primaryText>
          {"What will be the " |> ReasonReact.string}
          {
            llink(~href="d", [|"GDP" |> ReasonReact.string|])
            |> ReasonReact.element
          }
          {" of " |> ReasonReact.string}
          {
            llink(~href="China", [|"China" |> ReasonReact.string|])
            |> ReasonReact.element
          }
          {" in " |> ReasonReact.string}
          {
            llink(~href="2018", [|"2018" |> ReasonReact.string|])
            |> ReasonReact.element
          }
        </span>
      </div>
      {
        FC__StateStatus.make(
          ~state=OPEN(MomentRe.momentWithUnix(futureTime)),
          ~fontSize=`em(0.85),
          (),
        )
      }
    </Table.Cell>
    <Table.Cell
      flex=2
      styles=[
        Css.(style([paddingTop(`em(1.0)), paddingBottom(`em(0.5))])),
      ]>
      <FC__CdfChart__Small cdf minX=2.0 color={`hex("#d9dcdf")} maxX=12.0 />
    </Table.Cell>
    <Table.Cell flex=1 styles=[Css.(style([paddingTop(`em(0.5))]))]>
      <Div>
        <Link
          className=Css.(
            style([
              marginRight(`em(1.0)),
              color(Colors.textMedium),
              hover([color(Colors.textDark)]),
            ])
          )>
          {"Series A" |> ReasonReact.string}
        </Link>
        <Link
          className=Css.(
            style([
              marginRight(`em(1.0)),
              color(Colors.textMedium),
              hover([color(Colors.textDark)]),
            ])
          )>
          {"19" |> ReasonReact.string}
        </Link>
      </Div>
      <Div>
        <Link
          className=Css.(
            style([
              marginRight(`em(1.0)),
              textDecoration(`underline),
              color(Colors.textMedium),
              hover([color(Colors.textDark)]),
            ])
          )>
          {"Edit" |> ReasonReact.string}
        </Link>
        <Link
          className=Css.(
            style([
              marginRight(`em(1.0)),
              textDecoration(`underline),
              color(Colors.textMedium),
              hover([color(Colors.textDark)]),
            ])
          )>
          {"Archive" |> ReasonReact.string}
        </Link>
      </Div>
    </Table.Cell>
  </Table.RowLink>;

let make =
  <PageCard>
    {
      PageCard.header(
        <Div>
          <Div>
            <Tab2 isActive=true number=12>
              {"Open" |> ReasonReact.string}
            </Tab2>
            <Tab2 isActive=false number=18>
              {"Pending Resolution" |> ReasonReact.string}
            </Tab2>
            <Tab2 isActive=false number=831>
              {"Closed" |> ReasonReact.string}
            </Tab2>
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
        flex=4
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