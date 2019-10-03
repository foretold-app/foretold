open FC;
open Base;

let make =
  <PageCard>
    <PageCard.HeaderRow>
      <Div.Jsx2 float=`left>
        <PageCard.HeaderRow.Title>
          {"Pending Resolution" |> ReasonReact.string}
        </PageCard.HeaderRow.Title>
      </Div.Jsx2>
      <Div.Jsx2
        float=`right
        className={Css.style([
          PageCard.HeaderRow.Styles.itemTopPadding,
          PageCard.HeaderRow.Styles.itemBottomPadding,
        ])}>
        <FC__Button variant=Primary>
          {"< Back" |> ReasonReact.string}
        </FC__Button>
      </Div.Jsx2>
    </PageCard.HeaderRow>
    <Table>
      <Table.HeaderRow>
        <Table.Cell flex={`num(4.)}>
          {"Name & Status" |> ReasonReact.string}
        </Table.Cell>
        <Table.Cell flex={`num(2.)}>
          {"Aggregate Prediction" |> ReasonReact.string}
        </Table.Cell>
        <Table.Cell flex={`num(1.)}>
          {"Details" |> ReasonReact.string}
        </Table.Cell>
      </Table.HeaderRow>
    </Table>
  </PageCard>;