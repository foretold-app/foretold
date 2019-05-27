open FC;
open Base;

let make =
  <PageCard>
    <PageCard.HeaderRow>
      <PageCard.HeaderRow.Title>
        {"Pending Resolution" |> ReasonReact.string}
      </PageCard.HeaderRow.Title>
      <FC__Button variant=Primary>
        {"< Back" |> ReasonReact.string}
      </FC__Button>
    </PageCard.HeaderRow>
    <Table.HeaderRow>
      <Table.Cell flex=4> {"Name & Status" |> ReasonReact.string} </Table.Cell>
      <Table.Cell flex=2>
        {"Aggregate Prediction" |> ReasonReact.string}
      </Table.Cell>
      <Table.Cell flex=1> {"Details" |> ReasonReact.string} </Table.Cell>
    </Table.HeaderRow>
  </PageCard>;