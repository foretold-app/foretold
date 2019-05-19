open FC;
open Base;

let dist: Stats.dist = {
  xs: ExampleCdfs.Example1.xs,
  ys: ExampleCdfs.Example1.ys,
};

let llink =
  FC__Link.make(
    ~colors=(`hex("384e67"), Colors.link),
    ~isDisabled=false,
    ~styles=Css.(style([textDecoration(`underline)])),
  );

let row =
  <Table.Row>
    <Table.Cell
      flex=4
      styles=[
        Css.(style([paddingTop(`em(1.0)), paddingBottom(`em(0.5))])),
      ]>
      <div>
        <span className=Table.Row.primaryText>
          {"What will the " |> ReasonReact.string}
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
      <div>
        <span
          className=Css.(
            style([
              Colors.FontWeights.heavy,
              color(Colors.Statuses.green),
              marginRight(`em(1.0)),
              fontSize(`em(0.9)),
            ])
          )>
          {"Open" |> ReasonReact.string}
        </span>
        <span
          className=Css.(
            style([color(Colors.textMedium), fontSize(`em(0.9))])
          )>
          {"Closes in 8 days" |> ReasonReact.string}
        </span>
      </div>
    </Table.Cell>
    <Table.Cell
      flex=2
      styles=[
        Css.(style([paddingTop(`em(1.0)), paddingBottom(`em(0.5))])),
      ]>
      <FC__CdfChart__Small
        data={"xs": dist.xs, "ys": dist.ys}
        minX=2.0
        color={`hex("#d9dcdf")}
        maxX=12.0
      />
    </Table.Cell>
    <Table.Cell flex=1 styles=[Css.(style([paddingTop(`em(0.5))]))]>
      <Div>
        <Link
          styles=Css.(style([marginRight(`em(1.0))]))
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
          styles=Css.(
            style([
              Css.marginRight(`em(1.0)),
              Css.textDecoration(`underline),
            ])
          )>
          {"Edit" |> ReasonReact.string}
        </Link>
        <Link
          styles=Css.(style([Css.textDecoration(`underline)]))
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