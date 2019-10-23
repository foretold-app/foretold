open FC;
open Base;

let make =
  <PageCard.Jsx2>
    <PageCard.HeaderRow.Jsx2>
      <Div.Jsx2 float=`left>
        <PageCard.HeaderRow.Title.Jsx2>
          {"Pending Resolution" |> ReasonReact.string}
        </PageCard.HeaderRow.Title.Jsx2>
      </Div.Jsx2>
      <Div.Jsx2
        float=`right
        className={Css.style([
          PageCard.HeaderRow.Styles.itemTopPadding,
          PageCard.HeaderRow.Styles.itemBottomPadding,
        ])}>
        <FC__Button.Jsx2 variant=Primary size=Small>
          {"< Back" |> ReasonReact.string}
        </FC__Button.Jsx2>
      </Div.Jsx2>
    </PageCard.HeaderRow.Jsx2>
    <Table>
      <Table.HeaderRow.Jsx2>
        <Table.Cell flex={`num(4.)}>
          {"Name & Status" |> ReasonReact.string}
        </Table.Cell>
        <Table.Cell flex={`num(2.)}>
          {"Aggregate Prediction" |> ReasonReact.string}
        </Table.Cell>
        <Table.Cell flex={`num(1.)}>
          {"Details" |> ReasonReact.string}
        </Table.Cell>
      </Table.HeaderRow.Jsx2>
    </Table>
  </PageCard.Jsx2>;
